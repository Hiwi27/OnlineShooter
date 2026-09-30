// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletShell.generated.h"

UCLASS()
class BLASTER_API ABulletShell : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UStaticMeshComponent* ShellMesh;
	
	UPROPERTY(EditAnywhere, Category = "Bullet")
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float ShellEjectionRadialImpulse;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	USoundCue* ShellSound;

	FTimerHandle MyTimerHandle;
	
public:
	ABulletShell();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	private:
	void DestroyShell();
};
