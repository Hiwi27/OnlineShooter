// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BLASTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayText;

	void SetDisplayText(FString Text);

	UFUNCTION(BlueprintCallable)
	void ShowPLayerNetMode(APawn* InPawn);
	
	UFUNCTION(BlueprintCallable)
	void ShowPLayerNetName(APawn* InPawn);
	
protected:
	virtual void NativeDestruct() override;

};
