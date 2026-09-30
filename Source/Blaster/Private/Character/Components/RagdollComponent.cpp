// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Components/RagdollComponent.h"
#include "Blaster/BlasterTypes/DamageTypes.h"
#include "Camera/CameraComponent.h"
#include "Character/BlasterAnimInstance.h"
#include "Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"
#include "TimerManager.h"
#include "Character/Components/BlasterMovementComponent.h"

URagdollComponent::URagdollComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    LastSnapshotTime = 0.f;
    AuxSnapshotTime = 0.f;
}

void URagdollComponent::BeginPlay()
{
    Super::BeginPlay();
    BlasterCharacter = Cast<ABlasterCharacter>(GetOwner());
}

void URagdollComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void URagdollComponent::ActivateRagdoll()
{
	USkeletalMeshComponent* Mesh = BlasterCharacter->GetMesh();
	Mesh->SetIsReplicated(false);
    Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
    BlasterCharacter->SetActorEnableCollision(true);

    Mesh->SetAllBodiesSimulatePhysics(true);
    Mesh->SetSimulatePhysics(true);
    Mesh->WakeAllRigidBodies();
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->SetCollisionObjectType(ECC_PhysicsBody);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
    Mesh->bBlendPhysics = true;

    ApplyRagdollImpulse();

	//parte del la replicacion de ragdoll
	
	// **FORCE NET UPDATE**: forzamos al servidor a enviar el transform actual
	// BlasterCharacter->ForceNetUpdate();
	
    // GetWorld()->GetTimerManager().SetTimer(
    //     RagdollSnapshotTimer,
    //     this,
    //     &URagdollComponent::CaptureRagdollSnapshot,
    //     RagdollSnapInterval,
    //     true
    // );
}

void URagdollComponent::ApplyRagdollImpulse()
{
    // CaptureRagdollSnapshot();

    if (BlasterCharacter->LastDamage)
    {
        if (TCopyQualifiersFromTo_T<const UDamageType, UBulletDamage>* BulletDamage = Cast<UBulletDamage>(
	        BlasterCharacter->LastDamage))
        {
            FVector ImpulseDirection =
                (BulletDamage->HitResult.ImpactPoint - BulletDamage->HitResult.ImpactNormal).GetSafeNormal();
            BlasterCharacter->GetMesh()->AddImpulseAtLocation(
                ImpulseDirection * BulletDamage->DamageImpulse,
                BulletDamage->HitResult.ImpactPoint
            );
            DrawDebugSphere(GetWorld(), BulletDamage->HitResult.ImpactPoint, 16.f, 32, FColor::Yellow);
        }
    }
}

void URagdollComponent::CaptureRagdollSnapshot()
{
    if (!BlasterCharacter || !BlasterCharacter->HasAuthority())
        return;

    BlasterCharacter->SetAnimInstance(
        BlasterCharacter->GetAnimInstance() ?
            BlasterCharacter->GetAnimInstance() :
            Cast<UBlasterAnimInstance>(BlasterCharacter->GetMesh()->GetAnimInstance())
    );
	
    if (BlasterCharacter->GetAnimInstance())
    {
        FPoseSnapshot NewSnapshot;
        BlasterCharacter->GetAnimInstance()->SnapshotPose(NewSnapshot);
        FTransform NewComponentTransform = BlasterCharacter->GetMesh()->GetComponentTransform();

        if (!ReplicatedRagdollSnapshot.bIsValid ||
            IsSignificantChange(NewSnapshot, ReplicatedRagdollSnapshot, SignificantChangeTolerance))
        {
            ReplicatedRagdollSnapshot = NewSnapshot;
            ReplicatedComponentTransform = NewComponentTransform;
            Client_ReceiveRagdollSnapshot(NewSnapshot, NewComponentTransform);
        }
    }
}

void URagdollComponent::Client_ReceiveRagdollSnapshot_Implementation(
    const FPoseSnapshot& Snapshot,
    const FTransform& ComponentTransform)
{
	if (GetNetMode() != NM_Client)
		return;

    if (UBlasterAnimInstance* AnimInst = BlasterCharacter->GetAnimInstance())
	{
		// Mover la pose B previa a A:
		AnimInst->RagdollPoseA = AnimInst->RagdollPoseB;
		AnimInst->ComponentTransformA = AnimInst->ComponentTransformB;
		AnimInst->Rag_ReceiveTimeA = AnimInst->Rag_ReceiveTimeB;

		// Asignar nueva pose a B:
		AnimInst->RagdollPoseB = Snapshot;
		AnimInst->ComponentTransformB = ComponentTransform;
		AnimInst->Rag_ReceiveTimeB = GetWorld()->GetTimeSeconds();

		if (!AnimInst->RagdollPoseA.bIsValid)
		{
			// FreezeCameraOnDeath();
			if (USkeletalMeshComponent* Mesh = BlasterCharacter->GetMesh())
			{
				Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				Mesh->SetAllBodiesSimulatePhysics(false);
				Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				AnimInst->bHasTwoSnapshots = false;
			}
		}
		else
		{
			AnimInst->bHasTwoSnapshots = true;
		}

    	// Actualizar métricas de red para buffer dinámico:
    	float delta = AnimInst->Rag_ReceiveTimeB - AnimInst->Rag_ReceiveTimeA;
    	if (delta < 0.f) delta = 0.f;
    	const float MinBuffer = 0.02f;
    	const float MaxBuffer = 0.3f;
    	AnimInst->PlaybackBuffer = FMath::Clamp(delta, MinBuffer, MaxBuffer);
	}
}

bool URagdollComponent::IsSignificantChange(const FPoseSnapshot& A, const FPoseSnapshot& B, float Tolerance)
{
	if (!A.bIsValid || !B.bIsValid) return true;
	if (A.LocalTransforms.Num() != B.LocalTransforms.Num()) return true;
	for (int32 i = 0; i < A.LocalTransforms.Num(); ++i)
	{
		if (!A.LocalTransforms[i].Equals(B.LocalTransforms[i], Tolerance)) return true;
	}
	return false;
}
float URagdollComponent::GetInterpolationAlpha() const
{
	float Now = GetWorld()->GetTimeSeconds();
	float Interval = AuxSnapshotTime - LastSnapshotTime;
	float Elapsed = Now - LastSnapshotTime;

	return Interval > KINDA_SMALL_NUMBER ? FMath::Clamp(Elapsed / Interval, 0.f, 1.f) : 1.f;
}


void URagdollComponent::BeginDestroy()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
	
	Super::BeginDestroy();
}
