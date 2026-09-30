// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

class ABlasterPlayerController;
class ABlasterCharacter;
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

	TArray<ABlasterPlayerController*> PlayerControllers;

public:
	virtual void PlayerEliminated(ABlasterCharacter* EliminatedCharacter, ABlasterPlayerController* EliminatedController, ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(ABlasterCharacter* ElimmedCharacter, AController* ElimmedController);

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
