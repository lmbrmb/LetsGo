#pragma once

#include "LetsGo/Data/SharedValue.h"

class AmmoProvider : public SharedValue<int>
{
public:
	AmmoProvider(const int min, const int max, const int current);
};
