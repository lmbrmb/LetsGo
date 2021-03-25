#pragma once

#include "Item.h"
#include "LetsGo/Data/WeaponType.h"

///<summary>
/// Gun item
///</summary>
class LETSGO_API GunItem final : public Item
{
public:
	explicit GunItem(
		const FName& gunId,
		const WeaponType& gunType,
		const FName& ammoId,
		const int initialAmmoCount
	);

	int GetInitialAmmoCount() const;

	const WeaponType& GetGunType() const;

	const FName& GetAmmoId() const;

private:
	WeaponType _gunType;

	FName _ammoId;

	int _initialAmmoCount;
};
