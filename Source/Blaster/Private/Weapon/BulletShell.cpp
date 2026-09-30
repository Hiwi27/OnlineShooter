// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletShell.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ABulletShell::ABulletShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShellMesh");
	SetRootComponent(ShellMesh);
	ShellMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ShellMesh->SetSimulatePhysics(true);
	ShellMesh->SetNotifyRigidBodyCollision(true);
	ShellMesh->SetEnableGravity(true);

	ShellEjectionImpulse = 10.f;
}

// Called when the game starts or when spawned
void ABulletShell::BeginPlay()
{
	Super::BeginPlay();

	ShellMesh->OnComponentHit.AddDynamic(this, &ABulletShell::OnHit);
	ShellMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	ShellMesh->AddAngularImpulseInDegrees(GetActorUpVector() * ShellEjectionRadialImpulse);
}

void ABulletShell::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
		ShellMesh->OnComponentHit.RemoveDynamic(this ,&ABulletShell::OnHit);
	}

	GetWorldTimerManager().SetTimer(
		MyTimerHandle,
		this,
		&ABulletShell::DestroyShell,
		5.0f,
		false
	);
}

void ABulletShell::DestroyShell()
{
	GetWorldTimerManager().ClearTimer(MyTimerHandle);
	Destroy();
}

// Called every frame
void ABulletShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
