#pragma once

#include "EntityId.h"

/// <summary>
/// Weapon Id
/// </summary>
struct WeaponId final : public EntityId<FName>
{
public:
	WeaponId() : EntityId<FName>(FName())
	{
	};

	explicit WeaponId(const FName& id) : EntityId<FName>(id)
	{
	};

	virtual bool IsValid() const override
	{
		return !GetId().IsNone();
	}
};
