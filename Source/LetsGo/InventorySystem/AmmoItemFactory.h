#pragma once

#include "InventoryItemFactory.h"

class AmmoData
{
public:
	explicit AmmoData(const int quantity, const int maxQuantity) :
	Quantity(quantity), MaxQuantity(maxQuantity)
	{
		//Do nothing
	}

	int Quantity;

	int MaxQuantity;
};

////<summary>
///Ammo item factory.
///This factory treats ammo item as ammo box with fixed capacity and some initial bullets count inside.
///Only one ammo box can be carried in inventory, therefore ammo is limited.
///</summary>
class LETSGO_API AmmoItemFactory final : public InventoryItemFactory
{
public:
	AmmoItemFactory();
	
	~AmmoItemFactory();
	
	virtual InventoryItem* Create(const FName itemId) override;

private:
	TMap<FName, AmmoData> _ammoData;
};

Expose_TNameOf(AmmoItemFactory)
