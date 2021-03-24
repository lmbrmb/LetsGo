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
	explicit GunData(const FName& ammoId, const int initialAmmoCount) :
	AmmoId(ammoId),
	InitialAmmoCount(initialAmmoCount)
	{
	}

	FName AmmoId;
	
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
