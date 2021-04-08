#pragma once

#include "Misc/TypeContainer.h"

/// <summary>
/// [Interface] Provides Dependency Injection container.
/// </summary>
class IDiContainerProvider
{
public:
	virtual ~IDiContainerProvider() = default;

	virtual TTypeContainer<ESPMode::Fast>* GetDiContainer() const = 0;
};
