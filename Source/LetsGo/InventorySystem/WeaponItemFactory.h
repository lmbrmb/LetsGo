#pragma once

#include "InventoryItemFactory.h"

////<summary>
///Weapon item factory
///</summary>
class LETSGO_API WeaponItemFactory final : public InventoryItemFactory
{
public:
	WeaponItemFactory();
	
	~WeaponItemFactory();
	
	virtual InventoryItem* Create(FName itemId) override;

private:
	TArray<FName> _knownWeaponItems;
};

Expose_TNameOf(WeaponItemFactory)
