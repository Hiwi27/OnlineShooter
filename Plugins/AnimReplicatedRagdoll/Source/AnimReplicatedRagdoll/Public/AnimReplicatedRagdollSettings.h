// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AnimReplicatedRagdollSettings.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class ANIMREPLICATEDRAGDOLL_API UAnimReplicatedRagdollSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere)
	EVectorQuantization LocationQuantizationLevel = EVectorQuantization::RoundOneDecimal;

	// The bone rotations are what actually drives the replicated pose - the positions are
	// rebuilt from the reference skeleton - so their error accumulates down each limb.
	// ByteComponents only resolves 1.4 degrees, which is enough to visibly misplace a hand
	// or a foot after six or seven joints. Three extra bytes per bone buys 0.005 degrees.
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere)
	ERotatorQuantization RotationQuantizationLevel = ERotatorQuantization::ShortComponents;

	static UAnimReplicatedRagdollSettings* Get() { return GetMutableDefault<ThisClass>(); }
};
