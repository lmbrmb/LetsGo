#pragma once

#include "Item.h"

///<summary>
/// Gun item
///</summary>
class LETSGO_API GunItem final : public Item
{
public:
	explicit GunItem(
		const FName id,
		const FName ammoId,
		const int initialAmmoCount
	);

	int GetInitialAmmoCount() const;

	FName GetAmmoId() const;
	
private:
	FName _ammoId;
	
	int _initialAmmoCount;
};
