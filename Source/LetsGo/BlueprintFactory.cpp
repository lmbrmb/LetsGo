#include "BlueprintFactory.h"

#include "Utils/AssetUtils.h"

UBlueprint* BlueprintFactory::GetBlueprint(const FName id)
{
	if (Blueprints.Contains(id))
	{
		return Blueprints[id];
	}

	if (!Assets.Contains(id))
	{
		DevLogger::GetLoggingChannel()->LogValue("Unknown blueprint:" , id);
		return nullptr;
	}

	auto const assetPath = Assets[id];
	auto const blueprint = AssetUtils::LoadBlueprint(assetPath);
	Blueprints.Add(id, blueprint);
	return blueprint;
}

void BlueprintFactory::LoadAllBlueprints()
{
	for (auto const asset : Assets)
	{
		GetBlueprint(asset.Key);
	}
}
