// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BlasterMovementComponent.generated.h"


class ABlasterCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTER_API UBlasterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY()
	ABlasterCharacter* BlasterCharacter;
	
public:
	// UPROPERTY(ReplicatedUsing = OnRep_IsRagdollActive, EditAnywhere)
	// bool bIsRagdollActive = false;
	//
	UBlasterMovementComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ControlledCharacterMove(const FVector& InputVector, float DeltaSeconds) override;
	// virtual void ReplicateMoveToServer(float DeltaTime, const FVector& NewAcceleration) override;

protected:
	virtual void BeginPlay() override;

	// UFUNCTION()
	// void OnRep_IsRagdollActive();
};
