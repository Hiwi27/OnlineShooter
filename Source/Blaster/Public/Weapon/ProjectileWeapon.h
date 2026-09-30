// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "ProjectileWeapon.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<AProjectile> ProjectileClass;

public:
	virtual void Fire(const FVector& HitTarget) override;
};
