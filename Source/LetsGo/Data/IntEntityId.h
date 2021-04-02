#pragma once

#include "EntityId.h"

/// <summary>
/// Int entity Id
/// </summary>
struct IntEntityId : public EntityId<int>
{
public:
	IntEntityId() : EntityId<int>(MIN_int32)
	{
	};

	explicit IntEntityId(const int id) : EntityId<int>(id)
	{
	};

	virtual bool IsValid() const override
	{
		return GetId() != MIN_int32;
	}
};
