#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/DamageType.h"
#include "DamageTypes.generated.h"

UENUM(BlueprintType)
enum class EDamageTypes : uint8
{
	Bullet	UMETA(DisplayName = "BulletDamage"),
	Fire	UMETA(DisplayName = "Fire"),
	Unknown	UMETA(DisplayName = "Unknown")
};

UCLASS()
class BLASTER_API UBulletDamage : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Properties")
	FHitResult HitResult;
	
	UPROPERTY(EditAnywhere, Category="Properties")
	float Damage;
};

UCLASS()
class BLASTER_API UFireDamageType : public UDamageType
{
	GENERATED_BODY()
};

UCLASS()
class BLASTER_API UDamageTypes : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	static EDamageTypes GetDamageType(TSubclassOf<UDamageType> DamageTypeClass);
};