// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	UNiagaraSystem* Tracer;

	UPROPERTY()
	UNiagaraComponent* TracerComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	UNiagaraSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	float ImpactParticlesSize = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	USoundCue* ImpactSound;

	UPROPERTY()
	FVector_NetQuantizeNormal HitNormal;

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Combat")
	TSubclassOf<UDamageType> DamageType;
	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};