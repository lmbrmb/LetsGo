#include "GunItem.h"

GunItem::GunItem(
	const FName id,
	const FName ammoId,
	const int initialAmmoCount
)
	: Item(id),
	_ammoId(ammoId),
	_initialAmmoCount(initialAmmoCount)
{
	//Do nothing
}

int GunItem::GetInitialAmmoCount() const
{
	return _initialAmmoCount;
}

FName GunItem::GetAmmoId() const
{
	return _ammoId;
}
