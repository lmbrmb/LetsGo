#include "WeaponFactory.h"

WeaponFactory::WeaponFactory()
{
	Assets.Add("SawedOffShotgun", "/Game/Assets/Blueprints/Weapons/BP_SawedOffShotgun");
	Assets.Add("Minigun", "/Game/Assets/Blueprints/Weapons/BP_Minigun");
	Assets.Add("Railgun", "/Game/Assets/Blueprints/Weapons/BP_Railgun");
}
