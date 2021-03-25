#pragma once

#include "EntityId.h"

/// <summary>
/// Weapon type
/// </summary>
struct WeaponType final : public EntityId<FName>
{
public:
	WeaponType() : EntityId<FName>(FName())
	{
	};

	explicit WeaponType(const FName& id) : EntityId<FName>(id)
	{
	};

	virtual bool IsValid() const override
	{
		return !GetId().IsNone();
	}
};
