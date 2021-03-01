#pragma once

#include "GunItem.h"
#include "ItemFactory.h"

////<summary>
///Gun item factory
///</summary>
class LETSGO_API GunItemFactory final : public ItemFactory<GunItem>
{
class GunData
{
public:
	explicit GunData(const FName ammoType, const int initialAmmoCount) :
	AmmoType(ammoType),
	InitialAmmoCount(initialAmmoCount)
	{
		//Do nothing
	}

	FName AmmoType;
	
	int InitialAmmoCount;
};

public:
	GunItemFactory();

protected:
	virtual GunItem* CreateItem(const FName itemId) override;

private:
	TMap<FName, GunData> _gunData;
};

Expose_TNameOf(GunItemFactory)
