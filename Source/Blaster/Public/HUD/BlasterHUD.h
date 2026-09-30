// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

class UCombatOverlay;
class UTexture2D;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

	UPROPERTY()
	UTexture2D* CrosshairCenter;
	UPROPERTY()
	UTexture2D* CrosshairTop;
	UPROPERTY()
	UTexture2D* CrosshairRight;
	UPROPERTY()
	UTexture2D* CrosshairDown;
	UPROPERTY()
	UTexture2D* CrosshairLeft;
	UPROPERTY()
	float CrosshairSpread;
	UPROPERTY()
	FLinearColor CrosshairColor;
};


/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	TSubclassOf<UUserWidget> CombatOverlayClass;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
	
public:
	UPROPERTY()
	UCombatOverlay* CombatOverlay;

public:
	virtual void DrawHUD() override;

	FORCEINLINE void SetHUDPackage(const FHUDPackage &Package) { this->HUDPackage = Package; }

protected:
	virtual void BeginPlay() override;
	void AddCombatOverlay();

private:
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);
};
