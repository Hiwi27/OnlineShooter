// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUD/BlasterHUD.h"
#include "CombatComponent.generated.h"


class ABlasterHUD;
class ABlasterPlayerController;
class ABlasterCharacter;
class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	ABlasterCharacter* BlasterCharacter;

	UPROPERTY()
	ABlasterPlayerController* BlasterPlayerController;

	UPROPERTY()
	ABlasterHUD* BlasterHUD;
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY()
	bool bFireButtonPressed;

	/**
	 * HUD and Crosshairs
	 */
	
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	FVector HitTarget;
	FHUDPackage HUDPackage;

	/**
	 * AIMING AND FOV
	 */

	//Set to cameras FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ZoomInterpSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CrosshairsShrinkOnZoom = 0.58f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CrosshairsApertureOnShoot = 0.75f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CrosshairsInterpSpeed = 5.f;

	float CurrentFOV;

	/**
	 * Automatic Fire
	 */

	FTimerHandle FireTimer;

	bool bCanFire = true;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TraceLength;

	UCombatComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	friend class ABlasterCharacter;

	void EquipWeapon(AWeapon* Weapon);
	
protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	void Fire();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFireButtonPressed(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	void InterpFOV(float DeltaTime);

	void StartFireTimer();
	void FireTimerFinished();
};
