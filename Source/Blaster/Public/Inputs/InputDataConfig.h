// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputDataConfig.generated.h"
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLASTER_API UInputDataConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Move;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Look;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Jump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* EquipWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Crouch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Aim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Fire;
};
