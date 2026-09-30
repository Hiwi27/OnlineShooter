// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"

#include "Character/BlasterCharacter.h"
#include "Character/Components/BlasterMovementComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/BlasterHUD.h"
#include "HUD/CombatOverlay.h"

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetReplicateMovement(true);
	// if (ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetOwner()))
	// {
	// 	BlasterCharacter->GetBlasterMovementComponent()->bIsRagdollActive = false;
	// }
}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD && BlasterHUD->CombatOverlay && BlasterHUD->CombatOverlay->HealthBar && BlasterHUD->CombatOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CombatOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health),FMath::CeilToInt(MaxHealth));
		BlasterHUD->CombatOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void ABlasterPlayerController::SetHUDScore(float Score)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD && BlasterHUD->CombatOverlay && BlasterHUD->CombatOverlay->ScoreAmmount;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"),FMath::FloorToInt(Score));
		BlasterHUD->CombatOverlay->ScoreAmmount->SetText(FText::FromString(ScoreText));
	}
}
