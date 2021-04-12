#pragma once

#include "LetsGo/Data/IUObjectRegistry.h"
#include "Misc/TypeContainer.h"

template <ESPMode Mode>
class IDiContainerFactory
{
protected:
	~IDiContainerFactory() = default;

private:
	virtual TTypeContainer<Mode>* CreateContainer(IUObjectRegistry* uObjectRegistry) = 0;
};
