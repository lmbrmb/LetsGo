#include "GunFactory.h"

GunFactory::GunFactory(const bool lazyInitialization)
{
	Paths.Add("SawedOffShotgun", "/Game/Assets/Blueprints/Weapons/BP_SawedOffShotgun");
	Paths.Add("Minigun", "/Game/Assets/Blueprints/Weapons/BP_Minigun");
	Paths.Add("Railgun", "/Game/Assets/Blueprints/Weapons/BP_Railgun");

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
