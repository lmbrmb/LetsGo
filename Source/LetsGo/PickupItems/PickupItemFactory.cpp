#include "PickupItemFactory.h"
#include "LetsGo/Utils/AssetUtils.h"

PickupItemFactory::PickupItemFactory()
{
	_pickupAssets.Add("SawedOffShotgun", "/Game/Assets/Blueprints/PickupItems/BP_SawedOffShotgunPickupItem");
	_pickupAssets.Add("Minigun", "/Game/Assets/Blueprints/PickupItems/BP_MinigunPickupItem");
}

PickupItemFactory::~PickupItemFactory()
{
	//Do nothing
}

UBlueprint* PickupItemFactory::GetBlueprint(FName id)
{
	if (_pickupBlueprints.Contains(id))
	{
		return _pickupBlueprints[id];
	}

	auto const assetPath = _pickupAssets[id];
	auto const blueprint = AssetUtils::LoadBlueprint(assetPath);
	_pickupBlueprints.Add(id, blueprint);
	return blueprint;
}
