#pragma once

#include "Item.h"

///<summary>
/// Gun item
///</summary>
class LETSGO_API GunItem final : public Item
{
public:
	explicit GunItem(
		const FName& gunId,
		const FName& ammoId,
		const int initialAmmoCount
	);

	int GetInitialAmmoCount() const;

	const FName& GetAmmoId() const;
	
private:
	FName _ammoId;
	
	int _initialAmmoCount;
};
