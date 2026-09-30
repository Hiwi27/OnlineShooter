// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNode_ReplicatedRagdoll.h"

#include "Animation/AnimInstanceProxy.h"

// Turns a replicated pose into something LocalBlendCSBoneTransforms accepts: only bones
// present in the current LOD, ordered parents before children.
//
// Only the rotations are taken from the network. Each bone's position is rebuilt from its
// parent plus the bone offset of the reference skeleton, so bone lengths are the ones the
// skeleton was authored with and cannot drift - no amount of quantization error, packet
// loss or blending can stretch the mesh, because no length ever comes off the wire. The
// one exception is the root of the chain, whose replicated position places the whole body.
//
// Returns false when there is nothing to apply.
static bool BuildBoneTransforms(
	const FReplicatedRagdollData& Data,
	const FBoneContainer& BoneContainer,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	const int32 NumCompactBones = BoneContainer.GetCompactPoseNumBones();

	OutBoneTransforms.Reset(NumCompactBones);

	if (NumCompactBones == 0)
	{
		return false;
	}

	// Gather whatever arrived, indexed by compact bone so the hierarchy walk below can
	// address parents directly. A fast array makes no promise about its item order.
	TArray<FTransform> ComponentSpace;
	ComponentSpace.SetNum(NumCompactBones);

	TBitArray<> bHasData(false, NumCompactBones);

	for (const FReplicatedRagdollTransform& Item : Data.ComponentSpaceTransforms)
	{
		if (Item.BoneIndex == INDEX_NONE)
		{
			continue;
		}

		const FCompactPoseBoneIndex CompactIndex =
			BoneContainer.MakeCompactPoseIndex(
				FMeshPoseBoneIndex(Item.BoneIndex));

		if (!CompactIndex.IsValid())
		{
			continue;
		}

		ComponentSpace[CompactIndex.GetInt()] = Item.BoneTransform;
		bHasData[CompactIndex.GetInt()] = true;
	}

	// Compact pose indices are ordered parents before children, so one forward pass is
	// enough to have every parent already rebuilt by the time its children are reached.
	for (int32 Index = 0; Index < NumCompactBones; ++Index)
	{
		if (!bHasData[Index])
		{
			continue;
		}

		const FCompactPoseBoneIndex BoneIndex(Index);
		const FCompactPoseBoneIndex ParentIndex = BoneContainer.GetParentBoneIndex(BoneIndex);

		FTransform BoneCS;
		BoneCS.SetRotation(ComponentSpace[Index].GetRotation());
		BoneCS.SetScale3D(FVector::OneVector);

		if (ParentIndex.IsValid() && bHasData[ParentIndex.GetInt()])
		{
			BoneCS.SetLocation(
				ComponentSpace[ParentIndex.GetInt()].TransformPosition(
					BoneContainer.GetRefPoseTransform(BoneIndex).GetLocation()));
		}
		else
		{
			BoneCS.SetLocation(ComponentSpace[Index].GetLocation());
		}

		// Overwrite the gathered transform so children build off the rebuilt parent.
		ComponentSpace[Index] = BoneCS;

		OutBoneTransforms.Emplace(BoneIndex, BoneCS);
	}

	// Already ascending by compact index, which is the order the blend expects.
	return OutBoneTransforms.Num() > 0;
}

void FAnimNode_ReplicatedRagdoll::PreUpdate(const UAnimInstance* InAnimInstance)
{
	// cache the currently used skeletal mesh's bone names
	if (InAnimInstance->GetSkelMeshComponent() && InAnimInstance->GetSkelMeshComponent()->IsRegistered())
	{
		USkeletalMeshComponent* SkeletalMeshComponent = InAnimInstance->GetSkelMeshComponent();
		if (SkeletalMeshComponent)
		{
			for (TObjectPtr<USceneComponent> Component : SkeletalMeshComponent->GetAttachChildren())
			{
				if (UReplicatedRagdollComponent* RagdollComponent = Cast<UReplicatedRagdollComponent>(Component))
				{
					AnimDataHandle = RagdollComponent->GetAnimDataHandle();

					if (AnimDataHandle.IsValid())
					{
						if (AnimDataHandle->ReadCurrentRagdollData().ComponentSpaceTransforms.Num() > 0)
						{
							FReplicatedRagdollData CurrentData;
							CurrentData.CapturePose(SkeletalMeshComponent);
							AnimDataHandle->WriteCurrentRagdollData(CurrentData);
						}
						else
						{
							AnimDataHandle->WriteCurrentRagdollData(AnimDataHandle->ReadRagdollData());
						}
					}

					break;
				}
			}
		}

	}
}

void FAnimNode_ReplicatedRagdoll::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	FAnimNode_Base::Initialize_AnyThread(Context);

	ComponentPose.Initialize(Context);
}

void FAnimNode_ReplicatedRagdoll::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
	FAnimNode_Base::CacheBones_AnyThread(Context);
	//InitializeBoneReferences(Context.AnimInstanceProxy->GetRequiredBones());
	ComponentPose.CacheBones(Context);
}
void FAnimNode_ReplicatedRagdoll::EvaluateComponentSpace_AnyThread(
	FComponentSpacePoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentSpace_AnyThread)

	Super::EvaluateComponentSpace_AnyThread(Output);
	ComponentPose.EvaluateComponentSpace(Output);

	if (!AnimDataHandle.IsValid())
	{
		return;
	}

	if (!AnimDataHandle->ReadEvaluateAnimation())
	{
		return;
	}

	const FReplicatedRagdollData TargetData = AnimDataHandle->ReadRagdollData();

	if (TargetData.ComponentSpaceTransforms.Num() == 0)
	{
		return;
	}

	const FBoneContainer& BoneContainer =
		Output.Pose.GetPose().GetBoneContainer();

	TArray<FBoneTransform> BoneTransforms;

	// Start from the ragdoll pose we produced last frame instead of from the animation
	// graph's pose, so the blend below behaves as a recursive filter and converges.
	// PreUpdate captured it straight off the mesh, so it is a real, self consistent pose.
	if (BuildBoneTransforms(AnimDataHandle->ReadCurrentRagdollData(), BoneContainer, BoneTransforms))
	{
		Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, 1.f);
	}

	if (!BuildBoneTransforms(TargetData, BoneContainer, BoneTransforms))
	{
		return;
	}

	// With an alpha below 1 the engine blends in local space, where the translations are
	// the bone lengths and are identical in both poses. Interpolating the component space
	// positions instead would shorten and stretch the bones every frame.
	const float Alpha = FMath::Clamp(
		Output.AnimInstanceProxy->GetDeltaSeconds() * InterpSpeed, 0.f, 1.f);

	Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, Alpha);
}

void FAnimNode_ReplicatedRagdoll::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread);
	Super::Update_AnyThread(Context);

	GetEvaluateGraphExposedInputs().Execute(Context);

	ComponentPose.Update(Context);
}
