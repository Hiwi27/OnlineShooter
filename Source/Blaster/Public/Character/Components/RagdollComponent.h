#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RagdollComponent.generated.h"


struct FPoseSnapshot;
class ABlasterPlayerController;
class UBlasterAnimInstance;
class ABlasterCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTER_API URagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	float RagdollSnapInterval = 0.33f; // Intervalo de ejecucion de snapshot
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	float SignificantChangeTolerance = 0.2; // Tolerancia para mandar el snapshot
	float LastSnapshotTime = 0.f;
	float AuxSnapshotTime = 0.f;

private:
	UPROPERTY()
	ABlasterCharacter* BlasterCharacter;
	UPROPERTY()
	FPoseSnapshot ReplicatedRagdollSnapshot;
	UPROPERTY()
	FTransform ReplicatedComponentTransform;
	UPROPERTY()
	FTimerHandle RagdollSnapshotTimer;

public:
	URagdollComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ActivateRagdoll();
	void ApplyRagdollImpulse();
	float GetInterpolationAlpha() const;

protected:
	virtual void BeginPlay() override;
	UFUNCTION(NetMulticast, Reliable)
	void Client_ReceiveRagdollSnapshot(const FPoseSnapshot& Snapshot, const FTransform& ComponentTransform);
	void CaptureRagdollSnapshot();
	static bool IsSignificantChange(const FPoseSnapshot& A, const FPoseSnapshot& B, float Tolerance);
	virtual void BeginDestroy() override;
};
