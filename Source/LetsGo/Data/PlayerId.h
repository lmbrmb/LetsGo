#pragma once

#include "IntEntityId.h"

/// <summary>
/// Player Id
/// </summary>
struct PlayerId final : public IntEntityId
{
public:
	PlayerId() : IntEntityId()
	{
	};

	explicit PlayerId(const int id) : IntEntityId(id)
	{
	};
};
