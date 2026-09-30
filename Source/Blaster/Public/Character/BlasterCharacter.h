// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "GameFramework/Character.h"
#include "Interfaces/CrosshairsInteractInterface.h"
#include "BlasterCharacter.generated.h"

class UBlasterMovementComponent;
class ABlasterPlayerState;
class UReplicatedRagdollComponent;
struct FPoseSnapshot;
class UBlasterAnimInstance;
class ABlasterPlayerController;
class UCombatComponent;
class AWeapon;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UInputDataConfig;
struct FInputActionValue;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public ICrosshairsInteractInterface
{
	GENERATED_BODY()

	/** Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCombatComponent* CombatComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBlasterMovementComponent* MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Widgets")
	UWidgetComponent* OverheadWidget;

	/** Members */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* EliminationMontage;
	

public:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UReplicatedRagdollComponent* RagdollComponent;
    	
	/** Inputs */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputDataConfig* InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseEquippedSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseEquippedCrouchSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseAimSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CameraThreshold = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnThreshold = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	float RagdollSnapInterval = 0.05f; // El intervalo que usas en SetTimer
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	float SignificantChangeTolerance = 0.02; // El intervalo que usas en SetTimer
	UPROPERTY(EditAnywhere, Category = "Movement")
	const UDamageType* LastDamage;
	
	bool bPlayerEliminated = false;
	
private:
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	bool bRotateRootBone;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	ETurningInPlace TurningInPlace;
	float TimeSinceLastMovementReplication;

	/*
	 * Player Health
	 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_Health ,VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;
	
	float EliminationDelay = 3.f;

	UPROPERTY()
	FTimerHandle ElimTimer;

	UPROPERTY()
	UBlasterAnimInstance* AnimInstance;
	
	UPROPERTY()
	ABlasterPlayerController* BlasterPlayerController;

	UPROPERTY()
	ABlasterPlayerState* BlasterPlayerState;

public:
	explicit ABlasterCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	virtual void Destroyed() override;

	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();
	bool IsAiming();

	void PlayFireMontage(bool bAiming);
	void PlayHitReactMontage();
	// void PlayDeathMontage();
	
	virtual void OnRep_ReplicatedMovement() override;

	void Eliminated();
	
	UFUNCTION(NetMulticast, reliable)
	void MulticastEliminated();

	/** Solo servidor: activa la simulacion fisica del mesh. El resultado lo replica
	 *  UReplicatedRagdollComponent hueso a hueso. */
	void ActivateRagdoll();

	void FreezeCameraOnDeath();
	void EliminatedTimerFinished();

	/** Preparar el Character para ser destruido/respawneado (server) */
	// void PrepareForRespawn();
	
	//GETTERS
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE AWeapon* GetEquippedWeapon();
	FORCEINLINE FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE FTickFunction& GetPrimaryTick() { return PrimaryActorTick; }
	FORCEINLINE UBlasterAnimInstance* GetAnimInstance() { return AnimInstance; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	FORCEINLINE UBlasterMovementComponent* GetBlasterMovementComponent() const { return MovementComponent; }
	FORCEINLINE bool IsPlayerEliminated() const { return bPlayerEliminated; }
	//SETTERS
	FORCEINLINE void SetCameraBoom(USpringArmComponent* const InCameraBoom){this->CameraBoom = InCameraBoom;}
	FORCEINLINE void SetFollowCamera(UCameraComponent* const InCameraBoom){this->FollowCamera = InCameraBoom;}
	FORCEINLINE void SetAnimInstance(UBlasterAnimInstance* const InCameraBoom){this->AnimInstance = InCameraBoom;}

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PostInitializeComponents() override;

	virtual void Jump() override;

private:
	/** Input Actions */
	UFUNCTION()
	void MoveAction(const FInputActionValue& Value);
	
	UFUNCTION()
	void LookAction(const FInputActionValue& Value);
	
	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);
	
	UFUNCTION()
	void StopJumpAction(const FInputActionValue& Value);
	
	UFUNCTION()
	void EquipWeaponAction(const FInputActionValue& Value);
	
	UFUNCTION()
	void CrouchAction(const FInputActionValue& Value);
	
	UFUNCTION()
	void AimActionPressed(const FInputActionValue& Value);

	UFUNCTION()
	void AimActionReleased(const FInputActionValue& Value);

	UFUNCTION()
	void FireActionPressed(const FInputActionValue& Value);

	UFUNCTION()
	void FireActionReleased(const FInputActionValue& Value);
	
	void CalculateAO_Pitch();
	float CalculateSpeed();

	void AimOffset(float DeltaTime);
	void TurnInPlace(float DeltaTime);

	void SimProxiesTurn();
	void HideCameraIfCharacterClose();

	UFUNCTION()
	void RecieveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);


	void UpdateHUDHealth();

	void PollInit();

	/*
	 * NetWork
	 */
	// UFUNCTION(Server, Reliable) //part of the replicated ragdoll system
	// void ServerEliminated();
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	virtual void OnRep_IsCrouched() override;
		
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	UFUNCTION()
	void OnRep_Health();

	/** RPC al cliente para desactivar movimiento/inputs localmente antes de la destrucci�n */
	// UFUNCTION(Client, Reliable)
	// void ClientPrepareForRespawn();
};
