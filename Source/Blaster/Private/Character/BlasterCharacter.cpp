// Fill out your copyright notice in the Description page of Project Settings.


#include "Blaster/Public/Character/BlasterCharacter.h"

#include "AnimReplicatedRagdoll.h"
#include "BlasterPlayerController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ReplicatedRagdollComponent.h"
#include "Blaster/Blaster.h"
#include "Character/Components/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inputs/InputDataConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/BlasterGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "Character/BlasterAnimInstance.h"
#include "Character/BlasterPlayerState.h"
#include "Character/Components/BlasterMovementComponent.h"
#include "Character/Components/RagdollComponent.h"
#include "Engine/ActorChannel.h"


// Sets default values
ABlasterCharacter::ABlasterCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UBlasterMovementComponent>(CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	//
	// Components
	//
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, CameraBoom->SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	// RagdollComponent = CreateDefaultSubobject<URagdollComponent>(TEXT("RagdollComponent"));
	// RagdollComponent->SetIsReplicated(true);

	RagdollComponent = CreateDefaultSubobject<UReplicatedRagdollComponent>(TEXT("RagdollComponent"));
	RagdollComponent->SetupAttachment(GetMesh());

	MovementComponent = Cast<UBlasterMovementComponent>(GetCharacterMovement());
	MovementComponent->SetIsReplicated(true);

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SKeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	SetNetUpdateFrequency(120.f);
	SetMinNetUpdateFrequency(60.f);

	BaseWalkSpeed = 600.f;
	BaseEquippedSpeed = 400.f;
	BaseEquippedCrouchSpeed = 200.f;
	BaseAimSpeed = 200.f;
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->SetReplicateMovement(true);
	Health = 100.f;
	UpdateHUDHealth();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::RecieveDamage);
	}

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = BaseEquippedCrouchSpeed;

	BlasterPlayerController = BlasterPlayerController == nullptr
		                          ? Cast<ABlasterPlayerController>(GetController())
		                          : BlasterPlayerController;
	AnimInstance = AnimInstance == nullptr ? Cast<UBlasterAnimInstance>(GetMesh()->GetAnimInstance()) : AnimInstance;
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
	HideCameraIfCharacterClose();
	PollInit();
}

// Called to bind functionality to input
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//
	// EnhacedInput
	//
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!InputActions)
	{
		return;
	}

	Input->BindAction(InputActions->Move, ETriggerEvent::Triggered, this, &ABlasterCharacter::MoveAction);
	Input->BindAction(InputActions->Look, ETriggerEvent::Triggered, this, &ABlasterCharacter::LookAction);
	Input->BindAction(InputActions->Jump, ETriggerEvent::Started, this, &ABlasterCharacter::JumpAction);
	Input->BindAction(InputActions->Jump, ETriggerEvent::Completed, this, &ABlasterCharacter::StopJumpAction);
	Input->BindAction(InputActions->EquipWeapon, ETriggerEvent::Triggered, this, &ABlasterCharacter::EquipWeaponAction);
	Input->BindAction(InputActions->Crouch, ETriggerEvent::Started, this, &ABlasterCharacter::CrouchAction);
	Input->BindAction(InputActions->Aim, ETriggerEvent::Started, this, &ABlasterCharacter::AimActionPressed);
	Input->BindAction(InputActions->Aim, ETriggerEvent::Completed, this, &ABlasterCharacter::AimActionReleased);
	Input->BindAction(InputActions->Fire, ETriggerEvent::Started, this, &ABlasterCharacter::FireActionPressed);
	Input->BindAction(InputActions->Fire, ETriggerEvent::Completed, this, &ABlasterCharacter::FireActionReleased);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly)
	DOREPLIFETIME(ABlasterCharacter, Health)
}

bool ABlasterCharacter::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	// Esto hace que UE replique también tu movement component
	if (UActorComponent* MoveComp = GetCharacterMovement())
	{
		WroteSomething |= Channel->ReplicateSubobject(MoveComp, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void ABlasterCharacter::Destroyed()
{
	// if (HasAuthority())
	// {
	// 	GetWorldTimerManager().ClearTimer(RagdollSnapshotTimer);
	// }
	Super::Destroyed();
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->BlasterCharacter = this;
	}
}

void ABlasterCharacter::Jump()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else if (CanJump())
	{
		Super::Jump();
	}
}


AWeapon* ABlasterCharacter::GetEquippedWeapon()
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->EquippedWeapon;
}

FVector ABlasterCharacter::GetHitTarget() const
{
	if (CombatComponent == nullptr) return FVector();
	return CombatComponent->HitTarget;
}


void ABlasterCharacter::MoveAction(const FInputActionValue& Value)
{
	AddMovementInput(
		UKismetMathLibrary::GetRightVector(FRotator(0.f, GetControlRotation().Yaw, GetControlRotation().Roll)),
		Value.Get<FVector2D>().X);
	AddMovementInput(
		UKismetMathLibrary::GetForwardVector(FRotator(0.f, GetControlRotation().Yaw, GetControlRotation().Roll)),
		Value.Get<FVector2D>().Y);
}

void ABlasterCharacter::LookAction(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<FVector2D>().X);
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
}

void ABlasterCharacter::JumpAction(const FInputActionValue& Value)
{
	this->Jump();
}


void ABlasterCharacter::StopJumpAction(const FInputActionValue& Value)
{
	this->StopJumping();
}

void ABlasterCharacter::CrouchAction(const FInputActionValue& Value)
{
	if (!bIsCrouched)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ABlasterCharacter::AimActionPressed(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		CombatComponent->SetAiming(true);
	}
}

void ABlasterCharacter::AimActionReleased(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		CombatComponent->SetAiming(false);
	}
}

void ABlasterCharacter::FireActionPressed(const FInputActionValue& Value)
{
	if (IsWeaponEquipped())
	{
		CombatComponent->FireButtonPressed(true);
	}
}

void ABlasterCharacter::FireActionReleased(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
}


void ABlasterCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

float ABlasterCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;
	return Velocity.Size();
}

void ABlasterCharacter::AimOffset(float DeltaTime)
{
	if (CombatComponent && CombatComponent->EquippedWeapon == nullptr) return;

	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) //Standing Still, not jumping
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		// bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) //Running, or Jumping
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		// bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void ABlasterCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 5.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 5.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0;
}


void ABlasterCharacter::RecieveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                      AController* InstigatorController, AActor* DamageCauser)
{
	LastDamage = DamageType;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UpdateHUDHealth();
	PlayHitReactMontage();
	if (Health <= 0)
	{
		ABlasterGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ABlasterGameMode>();
		if (BlasterGameMode)
		{
			BlasterPlayerController = BlasterPlayerController != nullptr
				                          ? Cast<ABlasterPlayerController>(BlasterPlayerController)
				                          : BlasterPlayerController;
			ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			if (AttackerController)
			{
				BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
			}
		}
	}
}

void ABlasterCharacter::Eliminated()
{
	if (!HasAuthority())
		return;

	if (CombatComponent && CombatComponent->EquippedWeapon)
	{
		CombatComponent->EquippedWeapon->Dropped();
	}
	MulticastEliminated();
	ActivateRagdoll();
	GetWorldTimerManager().SetTimer(ElimTimer, this, &ABlasterCharacter::EliminatedTimerFinished, EliminationDelay);
}

void ABlasterCharacter::ActivateRagdoll()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	// El ragdoll solo simula en el servidor: UReplicatedRagdollComponent captura las
	// transforms en component space y las replica. Para que el cliente pueda aplicarlas
	// tal cual, el component transform del mesh tiene que quedarse donde está en vez de
	// seguir al body raiz, que es lo que haria SyncComponentToRBPhysics por defecto.
	CharacterMesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::ComponentTransformIsKinematic;

	CharacterMesh->SetCollisionObjectType(ECC_PhysicsBody);
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	CharacterMesh->SetSimulatePhysics(true);
}

void ABlasterCharacter::MulticastEliminated_Implementation()
{
	// Se pone en todas las maquinas, no solo en el servidor, para que la AnimBP
	// vea el mismo estado en cliente y en servidor.
	bPlayerEliminated = true;

	//Disable character movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	if (BlasterPlayerController)
	{
		DisableInput(BlasterPlayerController);
	}

	// La capsula muerta no debe estorbar a los vivos, y sobre todo no debe moverse:
	// el ragdoll replicado se aplica relativo al transform del mesh.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// El ragdoll viaja en component space, o sea relativo al transform del mesh, asi que
	// ese transform tiene que ser identico en servidor y en clientes o el cadaver aparece
	// desplazado.
	//
	// El network smoothing suaviza las correcciones de posicion desplazando el MESH
	// respecto a la capsula ("the mesh doesn't move, but the capsule does so we have a new
	// offset", CharacterMovementComponent::SmoothCorrection), y ese offset solo se
	// reabsorbe mientras el smoothing sigue corriendo. Al morir se queda congelado el que
	// hubiera en ese instante, y por eso el cuerpo flotaba solo en el cliente.
	//
	// Desactivado el smoothing, las correcciones van directas a la capsula, que aterriza
	// exactamente sobre la posicion congelada del servidor; y el mesh vuelve a su offset
	// de clase, que es el mismo en todas las maquinas.
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	GetMesh()->SetRelativeLocationAndRotation(GetBaseTranslationOffset(), GetBaseRotationOffset());
}

// void ABlasterCharacter::ServerEliminated_Implementation()
// {
// 	if (!HasAuthority()) return;
// 	
// 	SetReplicateMovement(false);
// 	
// 	FreezeCameraOnDeath();
// 	
// 	if (UBlasterMovementComponent* CharMove = GetBlasterMovementComponent())
// 	{
// 		CharMove->SetMovementMode(MOVE_None);
// 		CharMove->StopMovementImmediately();
// 		CharMove->DisableMovement();
// 		CharMove->bIsRagdollActive = true;
// 	}
// 	
// 	if (RagdollComponent)
// 	{
// 		RagdollComponent->ActivateRagdoll();
// 	}
// }

void ABlasterCharacter::EliminatedTimerFinished()
{
	if (CombatComponent == nullptr) return;
	if (ABlasterGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ABlasterGameMode>())

		{
		if (BlasterPlayerController)
		{
			BlasterGameMode->RequestRespawn(this, BlasterPlayerController);
		}
		else if (Controller)
		{
			BlasterGameMode->RequestRespawn(this, Controller);
		}
		else if (PreviousController)
		{
			BlasterGameMode->RequestRespawn(this, PreviousController);
		}
	}
}



void ABlasterCharacter::FreezeCameraOnDeath()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
		PC->SetViewTarget(this);
	}
	if (GetCameraBoom())
	{
		GetCameraBoom()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetCameraBoom()->bUsePawnControlRotation = false;
	}

	if (GetFollowCamera())
	{
		GetFollowCamera()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
	if (GetCameraBoom())
	{
		const FTransform CamTransform = GetCameraBoom()->GetComponentTransform();
		GetCameraBoom()->SetWorldTransform(CamTransform);
		GetCameraBoom()->SetComponentTickEnabled(false);
	}
	if (GetFollowCamera())
	{
		const FTransform CamTransform = GetFollowCamera()->GetComponentTransform();
		GetFollowCamera()->SetWorldTransform(CamTransform);
		GetFollowCamera()->SetComponentTickEnabled(false);
	}
}
void ABlasterCharacter::SimProxiesTurn()
{
	if (CombatComponent == nullptr && CombatComponent->EquippedWeapon == nullptr) return;
	bRotateRootBone = false;
	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;


	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	//Default Behavior
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ABlasterCharacter::EquipWeaponAction(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		if (HasAuthority())
		{
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = BaseEquippedSpeed;
			ServerEquipButtonPressed();
		}
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (CombatComponent && CombatComponent->EquippedWeapon);
}

bool ABlasterCharacter::IsAiming()
{
	return (CombatComponent && CombatComponent->bAiming);
}

void ABlasterCharacter::PlayFireMontage(bool bAiming)
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;

	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance = AnimInstance != nullptr ? AnimInstance : Cast<UBlasterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlasterCharacter::PlayHitReactMontage()
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;

	AnimInstance = AnimInstance != nullptr ? AnimInstance : Cast<UBlasterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

// void ABlasterCharacter::PlayDeathMontage()
// {
// 	AnimInstance = AnimInstance != nullptr ? AnimInstance : Cast<UBlasterAnimInstance>(GetMesh()->GetAnimInstance());
// 	if (AnimInstance && EliminationMontage)
// 	{
// 		AnimInstance->Montage_Play(EliminationMontage);
// 		FName SectionName("Body");
// 		AnimInstance->Montage_JumpToSection(SectionName);
// 	}
// }


void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
}

void ABlasterCharacter::OnRep_IsCrouched()
{
	Super::OnRep_IsCrouched();
}

void ABlasterCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	//Play hit reaction montage
	PlayHitReactMontage();
}


void ABlasterCharacter::UpdateHUDHealth()
{
	BlasterPlayerController = BlasterPlayerController == nullptr
		                          ? Cast<ABlasterPlayerController>(Controller)
		                          : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABlasterCharacter::PollInit()
{
	if (BlasterPlayerState == nullptr)
	{
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if (BlasterPlayerState)
		{
			BlasterPlayerState->AddToScore(0.f);
		}
	}
}

void ABlasterCharacter::HideCameraIfCharacterClose()
{
	if (!IsLocallyControlled()) return;
	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	{
		GetMesh()->SetVisibility(false);
		if (CombatComponent && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponMesh())
		{
			CombatComponent->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (CombatComponent && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponMesh())
		{
			CombatComponent->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}


// void ABlasterCharacter::PrepareForRespawn()
// {
// 	// Ejecutar en servidor
// 	if (!HasAuthority()) return;
//
// 	// Evitar replicación de movimiento
// 	SetReplicateMovement(false);
//
// 	// Parar movimiento en el servidor
// 	if (UBlasterMovementComponent* CharMove = GetBlasterMovementComponent())
// 	{
// 		CharMove->SetMovementMode(MOVE_None);
// 		CharMove->StopMovementImmediately();
// 		CharMove->DisableMovement();
// 		CharMove->bIsRagdollActive = false;
// 	}
//
// 	// Si había timers en RagdollComponent, limpiarlos
// 	if (RagdollComponent && GetWorld())
// 	{
// 		GetWorld()->GetTimerManager().ClearAllTimersForObject(RagdollComponent);
// 	}
//
// 	// Congelar cámara en servidor (si procede)
// 	FreezeCameraOnDeath();
//
// 	// Llamar al cliente propietario para que desactive localmente su movement y ticks inmediatamente
// 	ClientPrepareForRespawn();
//
// 	// Nota: No destruimos ni UnPossess aquí; GameMode seguirá con RestartPlayerAtPlayerStart y Destroy cuando corresponda.
// }

// void ABlasterCharacter::ClientPrepareForRespawn_Implementation()
// {
// 	// Ejecutar en cliente propietario: desactivar ticks, inputs y movimiento para evitar creación de SavedMoves
// 	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
// 	{
// 		MoveComp->SetMovementMode(MOVE_None);
// 		MoveComp->StopMovementImmediately();
// 		MoveComp->Deactivate();
// 		MoveComp->SetComponentTickEnabled(false);
// 		// DisableMovement() de CharacterMovementComponent es miembro protegido de PawnMovementComponent,
// 		// usamos StopMovementImmediately + Deactivate para asegurar que no simule.
// 	}
//
// 	// Desactivar entradas y colisión localmente
// 	DisableInput(nullptr);
// 	SetActorEnableCollision(false);
// 	SetActorHiddenInGame(true);
//
// 	// Opcional: dejar la cámara congelada localmente si no se ha hecho
// 	FreezeCameraOnDeath();
// }
