#pragma once

#include "NameEntityId.h"

/// <summary>
/// Skin Id
/// </summary>
struct SkinId final : public NameEntityId
{
	SkinId() : NameEntityId()
	{
	};

	explicit SkinId(const FName& id) : NameEntityId(id)
	{
	};

	explicit SkinId(const FString& id) : NameEntityId(FName(id))
	{
	};
};
