#include "MaterialFactory.h"

const FString MaterialFactory::MIXAMO_PATH_PREFIX = "/Game/Assets/Mixamo/";

MaterialFactory::MaterialFactory(const bool lazyInitialization)
{
	Paths.Add("Kachujin", MIXAMO_PATH_PREFIX + "Kachujin/MI_Kachujin");
	Paths.Add("Maria", MIXAMO_PATH_PREFIX + "Maria/MI_Maria");
	Paths.Add("Vampire", MIXAMO_PATH_PREFIX + "Vampire/MI_Vampire");

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}