#pragma once

#include "IntEntityId.h"

/// <summary>
/// Team Id
/// </summary>
struct TeamId final : public IntEntityId
{
	TeamId() : IntEntityId()
	{
	};

	explicit TeamId(const int id) : IntEntityId(id)
	{
	};
};
