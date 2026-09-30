// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectiles/ProjectileBullet.h"

#include "Blaster/BlasterTypes/DamageTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectileBullet::AProjectileBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (AController* OwnerController = OwnerCharacter->GetController())
		{
			switch (UDamageTypes::GetDamageType(DamageType))
			{
			case EDamageTypes::Bullet:
				{
					UBulletDamage* BulletDamage = DamageType->GetDefaultObject<UBulletDamage>();
					BulletDamage->HitResult = Hit;
					UGameplayStatics::ApplyDamage(OtherActor, BulletDamage->Damage, OwnerController, this, BulletDamage->GetClass());
					break;
				}
			case EDamageTypes::Fire:
				break;
			case EDamageTypes::Unknown:
				break;
			default: return;
			}
		}
	}
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

}

