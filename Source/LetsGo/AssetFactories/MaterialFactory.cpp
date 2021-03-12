#include "MaterialFactory.h"

const FString MaterialFactory::MIXAMO_PATH_PREFIX = "/Game/Assets/Mixamo/";

MaterialFactory::MaterialFactory(const bool lazyInitialization)
{
	Paths.Add("Kachujin", MIXAMO_PATH_PREFIX + "Kachujin/Kachujin_Material");
	Paths.Add("Maria", MIXAMO_PATH_PREFIX + "Maria/Maria_Material");
	Paths.Add("Vampire", MIXAMO_PATH_PREFIX + "Vampire/Vampire_Material");

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}