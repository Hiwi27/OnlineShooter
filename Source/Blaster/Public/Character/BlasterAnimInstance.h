// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterAnimInstance.generated.h"

struct FPoseSnapshot;
class AWeapon;
/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class ABlasterCharacter* BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bLocallyControlled;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bRotateRootBone;
	
	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	bool bEliminated;
	
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	FPoseSnapshot ReplicatedPoseSnapshot;

	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	FPoseSnapshot RagdollPoseA;
	
	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	FPoseSnapshot RagdollPoseB;

	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	FTransform ReplicatedComponentTransform;

	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	FTransform ComponentTransformA;
	
	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	FTransform ComponentTransformB;

	UPROPERTY(BlueprintReadWrite, Category = "Ragdoll", meta = (AllowPrivateAccess = "true"))
	bool bIsReplicatingRagdoll = false;

	
	
	float Rag_ReceiveTimeA = 0.f;
	float Rag_ReceiveTimeB = 0.f;
	bool bHasTwoSnapshots = false;

	float PlaybackBuffer = 0.2f;
	
	// Métricas para buffer dinámico:
	float AvgInterval = 0.2f;        // inicial aproximada (ej. 200 ms)
	float AvgJitter   = 0.f;
	const float AlphaEWMA = 0.1f;

	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void InterpPoseSnapshots(
	const FPoseSnapshot& PoseA,
	const FPoseSnapshot& PoseB, const FTransform& ComponentA,
	const FTransform& ComponentB, float Alpha, FPoseSnapshot& InOutCurrent,FTransform& OutComponentTransform 
);
};