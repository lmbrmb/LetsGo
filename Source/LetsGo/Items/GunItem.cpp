#include "GunItem.h"

GunItem::GunItem(
	const FName& gunId,
	const WeaponType& gunType,
	const FName& ammoId,
	const int initialAmmoCount
) : Item(gunId),
	_gunType(gunType),
	_ammoId(ammoId),
	_initialAmmoCount(initialAmmoCount)
{
}

int GunItem::GetInitialAmmoCount() const
{
	return _initialAmmoCount;
}

const WeaponType& GunItem::GetGunType() const
{
	return _gunType;
}

const FName& GunItem::GetAmmoId() const
{
	return _ammoId;
}
