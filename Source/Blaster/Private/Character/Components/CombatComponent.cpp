// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/CombatComponent.h"

#include "BlasterPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"



UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceLength = 50000.f;
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BlasterCharacter && BlasterCharacter->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BlasterCharacter)
	{
		if (BlasterCharacter->GetFollowCamera())
		{
			DefaultFOV = BlasterCharacter->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon)
	DOREPLIFETIME(UCombatComponent, bAiming)
}


void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (BlasterCharacter == nullptr || BlasterCharacter->Controller == nullptr) return;

	BlasterPlayerController = BlasterPlayerController == nullptr
		                          ? Cast<ABlasterPlayerController>(BlasterCharacter->Controller)
		                          : BlasterPlayerController;

	if (BlasterPlayerController)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(BlasterPlayerController->GetHUD()) : BlasterHUD;
		if (BlasterHUD)
		{
			if (EquippedWeapon != nullptr)
			{
				HUDPackage.CrosshairCenter = EquippedWeapon->GetCrosshairCenter();
				HUDPackage.CrosshairTop = EquippedWeapon->GetCrosshairTop();
				HUDPackage.CrosshairRight = EquippedWeapon->GetCrosshairRight();
				HUDPackage.CrosshairDown = EquippedWeapon->GetCrosshairDown();
				HUDPackage.CrosshairLeft = EquippedWeapon->GetCrosshairLeft();
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairTop = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairDown = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
			}

			//Calculate CrosshairSpread
			FVector2D WalkSpeedRange(0.f, BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = BlasterCharacter->GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange,
			                                                            Velocity.Size());

			if (BlasterCharacter->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			if (bAiming)
			{
				if (EquippedWeapon)
				{
					CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, CrosshairsShrinkOnZoom, DeltaTime, EquippedWeapon->GetZoomedInterpSpeed());
				}
				else
				{
					CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, CrosshairsShrinkOnZoom, DeltaTime, ZoomInterpSpeed);
				}
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0, DeltaTime, ZoomInterpSpeed);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f,DeltaTime, CrosshairsInterpSpeed);
			
			HUDPackage.CrosshairSpread = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
			BlasterHUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;

	if (bAiming)
	{
		//Interp using Weapon component params
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomedInterpSpeed());
	}
	else
	{
		//Interp using Combat component params
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	//Applying FOV to FollowCamera 
	if (BlasterCharacter && BlasterCharacter->GetFollowCamera())
	{
		BlasterCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || BlasterCharacter == nullptr) return;
	if (EquippedWeapon->IsAutomatic())
	{
		BlasterCharacter->GetWorldTimerManager().SetTimer(FireTimer,this,&UCombatComponent::FireTimerFinished, EquippedWeapon->GetCadency());
	}
}

void UCombatComponent::FireTimerFinished()
{
	bCanFire = true;
	if (bFireButtonPressed && EquippedWeapon && EquippedWeapon->IsAutomatic())
	{
		Fire();
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming; //Just for no delay
	ServerSetAiming(bIsAiming);
	if (BlasterCharacter && bIsAiming)
	{
		BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed = BlasterCharacter->BaseAimSpeed;
	}
	else
	{
		BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed = EquippedWeapon != nullptr
			                                                         ? BlasterCharacter->BaseEquippedSpeed
			                                                         : BlasterCharacter->BaseWalkSpeed;
	}
}


void UCombatComponent::Fire()
{
	if (bCanFire)
	{
		bCanFire = false;
		ServerFireButtonPressed(HitTarget);
		if (EquippedWeapon)
		{
			CrosshairShootingFactor += CrosshairsApertureOnShoot;
			CrosshairShootingFactor = FMath::Clamp(CrosshairShootingFactor, 0.f, 5.f);
		}
		StartFireTimer();
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if (bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (BlasterCharacter)
		{
			float DistanceToCharacter = (BlasterCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 25.f);
		}
		
		FVector End = CrosshairWorldPosition + CrosshairWorldDirection * TraceLength;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);
		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UCrosshairsInteractInterface>())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;
		}
	}
}

void UCombatComponent::ServerFireButtonPressed_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFire(TraceHitTarget);
}

void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr) return;
	if (BlasterCharacter)
	{
		BlasterCharacter->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (BlasterCharacter && bIsAiming)
	{
		BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed = BlasterCharacter->BaseAimSpeed;
	}
	else
	{
		BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed = EquippedWeapon != nullptr
			                                                         ? BlasterCharacter->BaseEquippedSpeed
			                                                         : BlasterCharacter->BaseWalkSpeed;
	}
}


void UCombatComponent::EquipWeapon(AWeapon* Weapon)
{
	if (BlasterCharacter == nullptr || Weapon == nullptr) return;
	BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed = BlasterCharacter->BaseEquippedSpeed;

	EquippedWeapon = Weapon;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* WeaponSocket = BlasterCharacter->GetMesh()->GetSocketByName(FName("hand_rSocket"));
	if (WeaponSocket)
	{
		WeaponSocket->AttachActor(EquippedWeapon, BlasterCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(BlasterCharacter);

	BlasterCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	BlasterCharacter->bUseControllerRotationYaw = true;
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && BlasterCharacter)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		const USkeletalMeshSocket* WeaponSocket = BlasterCharacter->GetMesh()->GetSocketByName(FName("hand_rSocket"));
		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(EquippedWeapon, BlasterCharacter->GetMesh());
		}
		BlasterCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		BlasterCharacter->bUseControllerRotationYaw = true;
	}
}
