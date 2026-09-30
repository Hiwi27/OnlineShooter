// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BlasterGameMode.h"

#include "BlasterPlayerController.h"
#include "Character/BlasterCharacter.h"
#include "Character/BlasterPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* EliminatedCharacter,
                                        ABlasterPlayerController* EliminatedController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* EliminatedPlayerState = EliminatedController ? Cast<ABlasterPlayerState>(EliminatedController->PlayerState) : nullptr;
	if (AttackerPlayerState && AttackerPlayerState != EliminatedPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Eliminated();
	}
}

void ABlasterGameMode::RequestRespawn(ABlasterCharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}

	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!HasAuthority() || !NewPlayer) return;

	ABlasterCharacter* BlasterChar = Cast<ABlasterCharacter>(NewPlayer->GetPawn());
	if (BlasterChar)
	{
		BlasterChar->SetOwner(NewPlayer);
		ABlasterPlayerController* BasterController = Cast<ABlasterPlayerController>(NewPlayer);
		if (BasterController)
		{
			PlayerControllers.Add(BasterController);
			UE_LOG(LogTemp, Log, TEXT("Ownership asignado en PostLogin para %s"),
				   *NewPlayer->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn no es ABlasterCharacter para %s"),
			   *NewPlayer->GetName());
	}
}
