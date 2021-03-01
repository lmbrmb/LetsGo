#pragma once

#include "AmmoItem.h"
#include "ItemFactory.h"

////<summary>
///Ammo item factory.
///This factory treats ammo item as ammo box with fixed capacity and some initial bullets count inside.
///Only one ammo box can be carried, therefore ammo is limited.
///</summary>
class LETSGO_API AmmoItemFactory final : public ItemFactory<AmmoItem>
{
class AmmoData
{
public:
	explicit AmmoData(const int quantity, const int maxQuantity) :
	Quantity(quantity),
	MaxQuantity(maxQuantity)
	{
		//Do nothing
	}

	int Quantity;

	int MaxQuantity;
};
	
public:
	AmmoItemFactory();
	
	~AmmoItemFactory() = default;

protected:
	virtual AmmoItem* CreateItem(const FName itemId) override;
	
private:
	TMap<FName, AmmoData> _ammoData;
};

Expose_TNameOf(AmmoItemFactory)
