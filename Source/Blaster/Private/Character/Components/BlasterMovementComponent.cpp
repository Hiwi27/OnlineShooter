// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/BlasterMovementComponent.h"

#include "Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UBlasterMovementComponent::UBlasterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBlasterMovementComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UBlasterMovementComponent, bIsRagdollActive)
}


// Called when the game starts
void UBlasterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	BlasterCharacter = Cast<ABlasterCharacter>(GetOwner());
	// ...
}

// void UBlasterMovementComponent::OnRep_IsRagdollActive()
// {
// 	if (BlasterCharacter)
// 	{
// 		BlasterCharacter->FreezeCameraOnDeath();
// 	}
// }


// Called every frame
void UBlasterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBlasterMovementComponent::ControlledCharacterMove(const FVector& InputVector, float DeltaSeconds)
{
	Super::ControlledCharacterMove(InputVector, DeltaSeconds);
}

// void UBlasterMovementComponent::ReplicateMoveToServer(float DeltaTime, const FVector& NewAcceleration)
// {
// 	if (!bIsRagdollActive)
// 	{
// 		Super::ReplicateMoveToServer(DeltaTime, NewAcceleration);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("MovmentComponentRagdoll ACTIVE"));
// 	}
// }
