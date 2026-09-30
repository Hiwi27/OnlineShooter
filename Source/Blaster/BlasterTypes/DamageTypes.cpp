#include "DamageTypes.h"

EDamageTypes UDamageTypes::GetDamageType(TSubclassOf<UDamageType> DamageTypeClass)
{
	if (!DamageTypeClass) return EDamageTypes::Unknown;

	if (DamageTypeClass->IsChildOf(UBulletDamage::StaticClass()))
	{
		return EDamageTypes::Bullet;
	}
	if (DamageTypeClass->IsChildOf(UFireDamageType::StaticClass()))
	{
		return EDamageTypes::Fire;
	}

	return EDamageTypes::Unknown;
}
