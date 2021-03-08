#include "PickupItemFactory.h"

PickupItemFactory::PickupItemFactory(const bool lazyInitialization)
	: BlueprintFactory()
{
	Assets.Add("SawedOffShotgun", "/Game/Assets/Blueprints/PickupItems/BP_SawedOffShotgunPickupItem");
	Assets.Add("Minigun", "/Game/Assets/Blueprints/PickupItems/BP_MinigunPickupItem");
	Assets.Add("Railgun", "/Game/Assets/Blueprints/PickupItems/BP_RailgunPickupItem");
	Assets.Add("MajorHealth", "/Game/Assets/Blueprints/PickupItems/BP_MajorHealthPickupItem");
	Assets.Add("MinorHealth", "/Game/Assets/Blueprints/PickupItems/BP_MinorHealthPickupItem");

	if (!lazyInitialization)
	{
		LoadAllBlueprints();
	}
}
