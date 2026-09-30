// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterPlayerState.h"

#include "BlasterPlayerController.h"
#include "Character/BlasterCharacter.h"


void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	float CurrentScore = GetScore();
	SetScore(CurrentScore + ScoreAmount);
	
	BlasterCharacter = BlasterCharacter == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : BlasterCharacter;
	if (BlasterCharacter)
	{
		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(BlasterCharacter->Controller) : BlasterPlayerController;
		if (BlasterPlayerController)
		{
			BlasterPlayerController->SetHUDScore(GetScore());
		}
	}
}

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	BlasterCharacter = BlasterCharacter == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : BlasterCharacter;
	if (BlasterCharacter)
	{
		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(BlasterCharacter->Controller) : BlasterPlayerController;
		if (BlasterPlayerController)
		{
			BlasterPlayerController->SetHUDScore(GetScore());
		}
	}
	
}

