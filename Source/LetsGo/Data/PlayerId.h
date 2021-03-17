#pragma once

#include "EntityId.h"

/// <summary>
/// Player Id
/// </summary>
struct PlayerId final : public EntityId<int>
{
public:
	PlayerId() : EntityId<int>(MIN_int32)
	{
	};

	explicit PlayerId(const int id) : EntityId<int>(id)
	{
	};

	virtual bool IsValid() const override
	{
		return GetId() != MIN_int32;
	}
};
