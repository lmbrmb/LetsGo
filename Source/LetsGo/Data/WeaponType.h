#pragma once

#include "NameEntityId.h"

/// <summary>
/// Weapon type
/// </summary>
struct WeaponType final : public NameEntityId
{
public:
	WeaponType() : NameEntityId()
	{
	};

	WeaponType(const FName& weaponType) : NameEntityId(weaponType)
	{
	};
};
