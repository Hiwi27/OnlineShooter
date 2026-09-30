// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedRagdollComponent.h"

#include "Engine/SkeletalMesh.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

DEFINE_LOG_CATEGORY_STATIC(LogReplicatedRagdoll, Log, All);

// Diagnostic: a ragdoll pose can only look deformed if some bone ends up at the wrong
// distance from its parent, so compare bone lengths against the reference skeleton at two
// points - the pose that arrived over the network, and the pose the mesh actually ends up
// rendering. Whichever of the two is off tells you whether the problem is the replicated
// data or something in the anim graph downstream of the ragdoll node.
static TAutoConsoleVariable<int32> CVarRagdollDebugBoneLengths(
	TEXT("ragdoll.DebugBoneLengths"),
	0,
	TEXT("Log the worst bone length error of the replicated ragdoll pose and of the final mesh pose."),
	ECVF_Default);

FReplicatedRagdollData FRagdollAnimData::ReadCurrentRagdollData() const
{
	FReadScopeLock Lock(DataLock);
	return CurrentData;
}

void FRagdollAnimData::WriteCurrentRagdollData(const FReplicatedRagdollData& NewData)
{
	FWriteScopeLock Lock(DataLock);
	CurrentData = NewData;
}

FReplicatedRagdollData FRagdollAnimData::ReadRagdollData() const
{
	FReadScopeLock Lock(DataLock);
	return Data;
}

void FRagdollAnimData::WriteRagdollData(const FReplicatedRagdollData& NewData)
{
	FWriteScopeLock Lock(DataLock);
	Data = NewData;
}

bool FRagdollAnimData::ReadEvaluateAnimation() const
{
	FReadScopeLock Lock(DataLock);
	return bEvaluateAnimation;
}

void FRagdollAnimData::WriteEvaluateAnimation(bool bNewEvaluateAnimation)
{
	FWriteScopeLock Lock(DataLock);
	bEvaluateAnimation = bNewEvaluateAnimation;
}

UReplicatedRagdollComponent::UReplicatedRagdollComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	SetIsReplicatedByDefault(true);
}

void UReplicatedRagdollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority() && GetNetMode() != ENetMode::NM_Standalone)
	{
		if (ShouldCaptureRagdoll())
		{
			CaptureRagdoll();
		}
		else
		{
			ClearRagdoll();
		}
	}

	AnimDataHandle->WriteEvaluateAnimation(ShouldApplyRagdoll());

	if (CVarRagdollDebugBoneLengths.GetValueOnGameThread() != 0)
	{
		DebugLogBoneLengths();
	}
}

// Worst |length - reference length| over every bone that has a parent, plus the bone it
// happened on. Poses are in component space, so the length is just the distance between a
// bone and its parent.
static void FindWorstBoneLength(
	const TArray<FTransform>& ComponentSpacePose,
	const FReferenceSkeleton& RefSkeleton,
	float& OutWorstError,
	FName& OutWorstBone)
{
	OutWorstError = 0.f;
	OutWorstBone = NAME_None;

	const TArray<FTransform>& RefBonePose = RefSkeleton.GetRefBonePose();

	for (int32 BoneIndex = 0; BoneIndex < ComponentSpacePose.Num(); ++BoneIndex)
	{
		const int32 ParentIndex = RefSkeleton.GetParentIndex(BoneIndex);

		if (ParentIndex == INDEX_NONE || !ComponentSpacePose.IsValidIndex(ParentIndex))
		{
			continue;
		}

		const float Length = FVector::Dist(
			ComponentSpacePose[BoneIndex].GetLocation(),
			ComponentSpacePose[ParentIndex].GetLocation());

		const float RefLength = RefBonePose[BoneIndex].GetLocation().Size();
		const float Error = FMath::Abs(Length - RefLength);

		if (Error > OutWorstError)
		{
			OutWorstError = Error;
			OutWorstBone = RefSkeleton.GetBoneName(BoneIndex);
		}
	}
}

void UReplicatedRagdollComponent::DebugLogBoneLengths() const
{
	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();

	if (SkeletalMesh == nullptr || SkeletalMesh->GetSkeletalMeshAsset() == nullptr || !AnimDataHandle.IsValid())
	{
		return;
	}

	const FReplicatedRagdollData ReplicatedPose = AnimDataHandle->ReadRagdollData();

	if (ReplicatedPose.ComponentSpaceTransforms.Num() == 0)
	{
		return;
	}

	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetSkeletalMeshAsset()->GetRefSkeleton();

	// Rebuild the replicated pose indexed by bone, since the fast array does not promise
	// that its item order matches the bone order.
	TArray<FTransform> ReplicatedByBone;
	ReplicatedByBone.SetNum(RefSkeleton.GetNum());

	for (const FReplicatedRagdollTransform& Item : ReplicatedPose.ComponentSpaceTransforms)
	{
		if (ReplicatedByBone.IsValidIndex(Item.BoneIndex))
		{
			ReplicatedByBone[Item.BoneIndex] = Item.BoneTransform;
		}
	}

	float ReplicatedError = 0.f;
	FName ReplicatedBone = NAME_None;
	FindWorstBoneLength(ReplicatedByBone, RefSkeleton, ReplicatedError, ReplicatedBone);

	float FinalError = 0.f;
	FName FinalBone = NAME_None;
	FindWorstBoneLength(SkeletalMesh->GetComponentSpaceTransforms(), RefSkeleton, FinalError, FinalBone);

	UE_LOG(LogReplicatedRagdoll, Warning,
		TEXT("[%s] replicated pose: worst %.2fcm on %s | final mesh pose: worst %.2fcm on %s"),
		GetOwner()->HasAuthority() ? TEXT("server") : TEXT("client"),
		ReplicatedError, *ReplicatedBone.ToString(),
		FinalError, *FinalBone.ToString());
}

void UReplicatedRagdollComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AnimData, Params);
}

void UReplicatedRagdollComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (!AnimDataHandle.IsValid())
	{
		AnimDataHandle = MakeShared<decltype(AnimDataHandle)::ElementType>();
	}
}

void UReplicatedRagdollComponent::Serialize(FArchive& Ar)
{
	if (Ar.IsSaving() && Ar.ArIsSaveGame)
	{
		CaptureRagdoll(false);
	}

	Super::Serialize(Ar);

	if (Ar.IsLoading() && Ar.ArIsSaveGame)
	{
		ApplyRagdoll();
	}
}

USkeletalMeshComponent* UReplicatedRagdollComponent::GetSkeletalMesh() const
{
	return Cast<USkeletalMeshComponent>(GetAttachParent());
}

void UReplicatedRagdollComponent::ClearRagdoll()
{
	if (!AnimData.ComponentSpaceTransforms.IsEmpty())
	{
		AnimData.ComponentSpaceTransforms.Empty();
		AnimData.MarkArrayDirty();

		AnimDataHandle->WriteRagdollData(AnimData);
	}
}

void UReplicatedRagdollComponent::CaptureRagdoll(bool bOptimizeCapture)
{
	if (USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh())
	{
		AnimData.CapturePose(SkeletalMesh, bOptimizeCapture);
		AnimDataHandle->WriteRagdollData(AnimData);
	}
}

void UReplicatedRagdollComponent::ApplyRagdoll()
{
	if (USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh())
	{
		// AnimData.ApplyPose(SkeletalMesh);
	}
}

bool UReplicatedRagdollComponent::ShouldApplyRagdoll_Implementation()
{
	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();

	// Where physics actually runs (the server) the mesh already holds the authoritative
	// pose. Applying the replicated data on top of it only fights the simulation, and it
	// leaks into the bones the physics asset has no body for.
	return SkeletalMesh != nullptr && !SkeletalMesh->IsAnySimulatingPhysics();
}

bool UReplicatedRagdollComponent::ShouldCaptureRagdoll_Implementation()
{
	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (SkeletalMesh == nullptr)
	{
		return false;
	}

	if (!SkeletalMesh->IsAnySimulatingPhysics())
	{
		return false;
	}

	return true;
}

void UReplicatedRagdollComponent::OnRep_AnimData()
{
	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (SkeletalMesh == nullptr)
	{
		return;
	}

	if (!AnimDataHandle.IsValid())
	{
		AnimDataHandle = MakeShared<decltype(AnimDataHandle)::ElementType>();
	}
	AnimDataHandle->WriteRagdollData(AnimData);
}
