#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UMovementComponent;
class UNiagaraSystem;
class ABulletShell;
class UWidgetComponent;
class USphereComponent;
class UTexture2D;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()

	/** Components */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* PickupWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UMovementComponent* MovementComp;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<ABulletShell> BulletShellClass;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float MuzzleFlashSize = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Combat")
	float Cadency = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Weapon Combat")
	bool bAutomaticFire = true;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	UTexture2D* CrosshairDown;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	UTexture2D* CrosshairLeft;

	//ZOOM FOV

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomedInterpSpeed = 20.f;
	
public:
	// GETTERS AND SETTERS
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE void SetWeaponMesh(USkeletalMeshComponent* Mesh) { WeaponMesh = Mesh; }

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE void SetAreaSphere(USphereComponent* Sphere) { AreaSphere = Sphere; }

	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE void SetPickupWidget(UWidgetComponent* Widget) { PickupWidget = Widget; }

	FORCEINLINE EWeaponState GetWeaponState() const { return WeaponState; }
	FORCEINLINE void SetWeaponState(EWeaponState State);

	FORCEINLINE UTexture2D* GetCrosshairCenter() const { return CrosshairCenter; }
	FORCEINLINE void SetCrosshairCenter(UTexture2D* Center){ CrosshairCenter = Center; }
	
	FORCEINLINE UTexture2D* GetCrosshairTop() const { return CrosshairTop; }
	FORCEINLINE void SetCrosshairTop(UTexture2D* Top){ CrosshairTop = Top; }
	
	FORCEINLINE UTexture2D* GetCrosshairRight() const { return CrosshairRight; }
	FORCEINLINE void SetCrosshairRight(UTexture2D* Right){ CrosshairRight = Right; }
	
	FORCEINLINE UTexture2D* GetCrosshairDown() const{ return CrosshairDown;}
	FORCEINLINE void SetCrosshairDown(UTexture2D* Down){ CrosshairDown = Down; }
	
	FORCEINLINE UTexture2D* GetCrosshairLeft() const{ return CrosshairLeft;}
	FORCEINLINE void SetCrosshairLeft(UTexture2D* Left){ CrosshairLeft = Left; }

	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomedInterpSpeed() const { return ZoomedInterpSpeed; }

	FORCEINLINE float GetCadency() const { return Cadency; }
	FORCEINLINE float IsAutomatic() const { return bAutomaticFire; }

	AWeapon();

	virtual void Tick(float DeltaTime) override;

	void ShowPickupWidget(bool bShowWidget);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Fire(const FVector& HitTarget);

	void Dropped();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UFUNCTION()
	void OnRep_WeaponState();

};
