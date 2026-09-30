// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectiles/Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Blaster/Blaster.h"
#include "Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility,ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic,ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_PhysicsBody,ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SKeletalMesh,ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Tracer,CollisionBox,FName("Tracer"),GetActorLocation(),GetActorRotation(),EAttachLocation::Type::KeepWorldPosition,false);
		// UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox,FName(TEXT("Tracer")), GetActorLocation(),GetActorRotation(), EAttachLocation::Type::KeepWorldPosition);
	}

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
	
}

// OnHit Events only register on the server
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	HitNormal = Hit.ImpactNormal;	
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	
	if (ImpactParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, GetActorLocation(), HitNormal.Rotation(), FVector(ImpactParticlesSize),true);
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

