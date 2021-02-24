#include "BlueprintFactory.h"
#include "Logs/DevLogger.h"
#include "Utils/AssetUtils.h"

BlueprintFactory::BlueprintFactory()
{
	//Do nothing
}

BlueprintFactory::~BlueprintFactory()
{
	//Do nothing
}

UBlueprint* BlueprintFactory::GetBlueprint(FName id)
{
	if (Blueprints.Contains(id))
	{
		return Blueprints[id];
	}

	if (!Assets.Contains(id))
	{
		DevLogger::GetLoggingChannel()->LogValue("Factory does not contain asset id:", id);
		return nullptr;
	}

	auto const assetPath = Assets[id];
	auto const blueprint = AssetUtils::LoadBlueprint(assetPath);
	Blueprints.Add(id, blueprint);
	return blueprint;
}
