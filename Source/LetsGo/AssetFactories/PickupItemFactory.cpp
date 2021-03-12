#include "PickupItemFactory.h"

PickupItemFactory::PickupItemFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", "/Game/Assets/Blueprints/PickupItems/BP_SawedOffShotgunPickupItem");
	Paths.Add("Minigun", "/Game/Assets/Blueprints/PickupItems/BP_MinigunPickupItem");
	Paths.Add("Railgun", "/Game/Assets/Blueprints/PickupItems/BP_RailgunPickupItem");
	Paths.Add("MajorHealth", "/Game/Assets/Blueprints/PickupItems/BP_MajorHealthPickupItem");
	Paths.Add("MinorHealth", "/Game/Assets/Blueprints/PickupItems/BP_MinorHealthPickupItem");

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
