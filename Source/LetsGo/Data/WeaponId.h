#pragma once

#include "NameEntityId.h"

/// <summary>
/// Weapon Id
/// </summary>
struct WeaponId final : public NameEntityId
{
	WeaponId() : NameEntityId()
	{
	};

	explicit WeaponId(const FName& id) : NameEntityId(id)
	{
	};
};
