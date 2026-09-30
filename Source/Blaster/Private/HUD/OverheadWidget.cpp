// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString Text)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(Text));
	}
}

void UOverheadWidget::ShowPLayerNetMode(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole)
	{
	case ROLE_None:
		Role = "None";
		break;
	case ROLE_SimulatedProxy:
		Role = "SimulatedProxy";
		break;
	case ROLE_AutonomousProxy:
		Role = "AutonomousProxy";
		break;
	case ROLE_Authority:
		Role = "Authority";
		break;
	case ROLE_MAX:
		Role = "Max";
		break;
	default: ;
		Role = "";
	}

	SetDisplayText(FString::Printf(TEXT("Local Role: %s"), *Role));
}

void UOverheadWidget::ShowPLayerNetName(APawn* InPawn)
{
	APlayerState* PlayerState = InPawn->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		FString PlayerName = PlayerState->GetPlayerName();
		if (DisplayName)
		{
			DisplayName->SetText(FText::FromString(PlayerName));
		}
	}

}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();

	Super::NativeDestruct();
}
