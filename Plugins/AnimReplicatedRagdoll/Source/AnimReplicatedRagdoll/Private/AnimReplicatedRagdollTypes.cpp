// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimReplicatedRagdollTypes.h"

#include "RRSkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Net/Core/Serialization/QuantizedVectorSerialization.h"
#include "AnimReplicatedRagdollSettings.h"

// Returns how much location/rotation difference there must be for a bone's transform to get replicated
static TPair<float, float> GetMinTransformDelta(EVectorQuantization VectorQuantization, ERotatorQuantization RotationQuantization)
{
	float VectorDelta = 1.f;
	float RotationDelta = 1.f;

	switch (VectorQuantization)
	{
	case EVectorQuantization::RoundTwoDecimals:
		VectorDelta = 0.01f;
		break;
	case EVectorQuantization::RoundOneDecimal:
		VectorDelta = 0.1f;
		break;
	case EVectorQuantization::RoundWholeNumber:
		VectorDelta = 1.f;
		break;
	}

	switch (RotationQuantization)
	{
	case ERotatorQuantization::ByteComponents:
		RotationDelta = 360.f / 256.f;
		break;

	case ERotatorQuantization::ShortComponents:
		RotationDelta = 360.f / 65536.f;
		break;
	}

	return { VectorDelta, RotationDelta };
}

void FReplicatedRagdollData::CapturePose(const USkeletalMeshComponent* SkeletalMesh, bool bOptimizeCapture)
{
	check(SkeletalMesh != nullptr);

	const TArray<FTransform>& BoneTransforms =
		SkeletalMesh->GetComponentSpaceTransforms();

	const int32 NumBones = BoneTransforms.Num();

	const ENetMode NetMode = SkeletalMesh->GetNetMode();

	if (bOptimizeCapture &&
		(NetMode == ENetMode::NM_DedicatedServer || NetMode == ENetMode::NM_ListenServer) &&
		(ComponentSpaceTransforms.Num() == NumBones))
	{
		for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
		{
			FReplicatedRagdollTransform& Item = ComponentSpaceTransforms[BoneIndex];
			const FTransform& NewTransform = BoneTransforms[BoneIndex];

			const TPair<float, float> Epsilon =
				GetMinTransformDelta(
					UAnimReplicatedRagdollSettings::Get()->LocationQuantizationLevel,
					UAnimReplicatedRagdollSettings::Get()->RotationQuantizationLevel);

			const bool bLocationEqual =
				(NewTransform.GetLocation() - Item.BoneTransform.GetLocation())
				.IsNearlyZero(Epsilon.Key);

			const bool bRotationEqual =
				(NewTransform.GetRotation().Rotator() - Item.BoneTransform.GetRotation().Rotator())
				.IsNearlyZero(Epsilon.Value);

			if (!bLocationEqual || !bRotationEqual)
			{
				Item.BoneTransform = NewTransform;
				Item.BoneIndex = BoneIndex; // IMPORTANTE
				MarkItemDirty(Item);
			}
		}
	}
	else
	{
		ComponentSpaceTransforms.SetNum(NumBones);

		for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
		{
			ComponentSpaceTransforms[BoneIndex].BoneIndex = BoneIndex;
			ComponentSpaceTransforms[BoneIndex].BoneTransform = BoneTransforms[BoneIndex];
		}

		MarkArrayDirty();
	}
}

void FReplicatedRagdollData::ApplyPose(USkeletalMeshComponent* SkeletalMesh)
{
	check(SkeletalMesh != nullptr);

	TArray<FTransform>& BoneTransforms =
		SkeletalMesh->GetEditableComponentSpaceTransforms();

	// 🔥 IMPORTANTE: reset o invalidación controlada
	if (BoneTransforms.Num() == 0)
	{
		return;
	}

	for (const FReplicatedRagdollTransform& Item : ComponentSpaceTransforms)
	{
		const int32 BoneIndex = Item.BoneIndex;

		if (!BoneTransforms.IsValidIndex(BoneIndex))
		{
			continue;
		}

		BoneTransforms[BoneIndex] = Item.BoneTransform;
	}

	((URRSkeletalMeshComponent*)SkeletalMesh)->ApplyEditedComponentSpaceTransforms();
}

bool FReplicatedRagdollTransform::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// The receiving side has no way to infer this: a fast array does not guarantee
	// that the item order matches the sender's.
	uint32 PackedBoneIndex = static_cast<uint32>(BoneIndex);
	Ar.SerializeIntPacked(PackedBoneIndex);

	if (Ar.IsLoading())
	{
		BoneIndex = static_cast<int32>(PackedBoneIndex);
	}

	FRotator BoneRotation = BoneTransform.GetRotation().Rotator();

	const UAnimReplicatedRagdollSettings* Settings = UAnimReplicatedRagdollSettings::Get();

	switch (Settings->RotationQuantizationLevel)
	{
	case ERotatorQuantization::ByteComponents:
		BoneRotation.SerializeCompressed(Ar);
		break;

	case ERotatorQuantization::ShortComponents:
		BoneRotation.SerializeCompressedShort(Ar);
		break;

	default:
		break;
	}

	if (Ar.IsLoading())
	{
		BoneTransform.SetRotation(BoneRotation.Quaternion());
	}

	FVector Location = BoneTransform.GetLocation();

	switch (Settings->LocationQuantizationLevel)
	{
	case EVectorQuantization::RoundWholeNumber:
		SerializePackedVector<1, 24>(Location, Ar);
		break;

	case EVectorQuantization::RoundOneDecimal:
		SerializePackedVector<10, 27>(Location, Ar);
		break;

	case EVectorQuantization::RoundTwoDecimals:
		SerializePackedVector<100, 30>(Location, Ar);
		break;

	default:
		break;
	}

	if (Ar.IsLoading())
	{
		BoneTransform.SetLocation(Location);
	}

	return true;
}
