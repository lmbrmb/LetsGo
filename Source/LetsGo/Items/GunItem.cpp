#include "GunItem.h"

GunItem::GunItem(
	const FName& gunId,
	const FName& ammoId,
	const int initialAmmoCount
) :	Item(gunId),
	_ammoId(ammoId),
	_initialAmmoCount(initialAmmoCount)
{
}

int GunItem::GetInitialAmmoCount() const
{
	return _initialAmmoCount;
}

const FName& GunItem::GetAmmoId() const
{
	return _ammoId;
}
