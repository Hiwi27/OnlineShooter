// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/BlasterCharacter.h"
#include "Animation/PoseSnapshot.h"
#include "Character/Components/RagdollComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Weapon.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter == nullptr) return;

	FVector GroundVelocity = BlasterCharacter->GetVelocity();
	Velocity = GroundVelocity;
	GroundVelocity.Z = 0.f;
	GroundSpeed = GroundVelocity.Size();

	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();
	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

	bIsCrouched = BlasterCharacter->bIsCrouched;
	bShouldMove = bIsAccelerating && GroundSpeed > 0.f;
	bAiming = BlasterCharacter->IsAiming();
	TurningInPlace = BlasterCharacter->GetTurningInPlace();
	bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();
	bEliminated = BlasterCharacter->IsPlayerEliminated();


	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;


	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BlasterCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = BlasterCharacter->GetAO_Yaw();
	AO_Pitch = BlasterCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(),
		                                                  FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (BlasterCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(
				FName("hand_r"), RTS_World);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				RightHandTransform.GetLocation(),
				RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->
					GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 100.f);
		}
	}

	// bIsReplicatingRagdoll y ReplicatedPoseSnapshot son del sistema de ragdoll viejo, que
	// ya no rellena nadie. El ragdoll lo aplica ahora el nodo Replicated Ragdoll a partir
	// de lo que replica UReplicatedRagdollComponent. Se deja a false para que el nodo
	// Pose Snapshot del AnimGraph no llegue a activarse con una pose vacia; ambas
	// propiedades se borran en cuanto ese nodo salga del ABP_Blaster.
	bIsReplicatingRagdoll = false;

	// /*
	//  * CLIENT RAGDOLL
	//  */
	// if (BlasterCharacter->GetNetMode() != NM_Client)
	// 	return;
	//
	// // Si solo B existe (primera snapshot), aplicar pose B estática
	// if (!bHasTwoSnapshots && RagdollPoseB.bIsValid)
	// {
	// 	if (USkeletalMeshComponent* Mesh = BlasterCharacter->GetMesh())
	// 	{
	// 		Mesh->SetWorldTransform(ComponentTransformB);
	// 		ReplicatedPoseSnapshot = RagdollPoseB;
	// 		return;
	// 	}
	// }
	// // Validar snapshots y transforms disponibles
	// if (!RagdollPoseA.bIsValid || !RagdollPoseB.bIsValid ||
	// 	!ComponentTransformA.IsValid() || !ComponentTransformB.IsValid())
	// {
	// 	return;
	// }
	//
	// float Now = GetWorld()->GetTimeSeconds();
	// float RenderTime = Now - PlaybackBuffer;
	// // Calcular intervalo A→B
	// float Interval = Rag_ReceiveTimeB - Rag_ReceiveTimeA;
	// const float MinInterval = 0.001f;
	// if (Interval < MinInterval) Interval = MinInterval;
	//
	// float Elapsed = RenderTime - Rag_ReceiveTimeA;
	// float Alpha = Elapsed / Interval;
	// Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	//
	// UE_LOG(LogTemp, Verbose, TEXT("Ragdoll: Now=%.3f, RT=%.3f, A=%.3f, B=%.3f, Interval=%.3f, Elapsed=%.3f, Alpha=%.3f, Buffer=%.3f"),
	//     Now, RenderTime, Rag_ReceiveTimeA, Rag_ReceiveTimeB, Interval, Elapsed, Alpha, PlaybackBuffer);
	//
	// // Interpolar
	// FPoseSnapshot InterpolatedPose;
	// FTransform InterpolatedCompTransform;
	// InterpPoseSnapshots(
	// 	RagdollPoseA, RagdollPoseB,
	// 	ComponentTransformA, ComponentTransformB,
	// 	Alpha,
	// 	InterpolatedPose,
	// 	InterpolatedCompTransform
	// );
	// // Aplicar transform y pose
	// if (USkeletalMeshComponent* Mesh = BlasterCharacter->GetMesh())
	// {
	// 	Mesh->SetWorldTransform(InterpolatedCompTransform);
	// 	ReplicatedPoseSnapshot = InterpolatedPose;
	// 	bIsReplicatingRagdoll = true;
	// }
	// if (Alpha >= 1.0f)
	// {
	// 	bHasTwoSnapshots = false;
	// }
	
}

// void UBlasterAnimInstance::InterpPoseSnapshots(const FPoseSnapshot& PoseA,
//                                                const FPoseSnapshot& PoseB, const FTransform& ComponentA,
//                                                const FTransform& ComponentB, float Alpha, FPoseSnapshot& InOutCurrent,
//                                                FTransform& OutComponentTransform)
// {
// 	int32 NumBones = PoseA.LocalTransforms.Num();
// 	if (!PoseA.bIsValid || !PoseB.bIsValid ||
// 		NumBones != PoseB.LocalTransforms.Num() ||
// 		PoseA.BoneNames.Num() != PoseB.BoneNames.Num())
// 	{
// 		return;
// 	}
//
// 	// Init de Out si es la primera vez
// 	if (!InOutCurrent.bIsValid)
// 	{
// 		InOutCurrent = PoseA;
// 		InOutCurrent.LocalTransforms.SetNum(NumBones);
// 		InOutCurrent.BoneNames = PoseA.BoneNames;
// 		InOutCurrent.SkeletalMeshName = PoseA.SkeletalMeshName;
// 		InOutCurrent.SnapshotName = FName(TEXT("BlendedSnapshot"));
// 		InOutCurrent.bIsValid = true;
// 	}
//
// 	for (int32 i = 1; i < NumBones; ++i) // empezamos en 1 porque 0 es el root
// 	{
// 		const FTransform& TA = PoseA.LocalTransforms[i];
// 		const FTransform& TB = PoseB.LocalTransforms[i];
// 		FTransform& TC = InOutCurrent.LocalTransforms[i];
//
// 		TC.SetLocation(FMath::Lerp(TA.GetLocation(), TB.GetLocation(), Alpha));
// 		TC.SetRotation(FQuat::Slerp(TA.GetRotation(), TB.GetRotation(), Alpha));
// 		TC.SetScale3D(FMath::Lerp(TA.GetScale3D(), TB.GetScale3D(), Alpha));
// 	}
//
//
// 	OutComponentTransform.SetLocation(
// 		FMath::Lerp(ComponentA.GetLocation(), ComponentB.GetLocation(), Alpha)
// 	);
// 	OutComponentTransform.SetRotation(
// 		FQuat::Slerp(ComponentA.GetRotation(), ComponentB.GetRotation(), Alpha)
// 	);
// 	OutComponentTransform.SetScale3D(
// 		FMath::Lerp(ComponentA.GetScale3D(), ComponentB.GetScale3D(), Alpha)
// 	);
// }
