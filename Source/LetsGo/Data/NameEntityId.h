#pragma once

#include "EntityId.h"

/// <summary>
/// Name entity Id
/// </summary>
struct NameEntityId : public EntityId<FName>
{
public:
	NameEntityId() : EntityId<FName>(FName())
	{
	};

	explicit NameEntityId(const FName& id) : EntityId<FName>(id)
	{
	};

	virtual bool IsValid() const override
	{
		return !GetId().IsNone();
	}
};
