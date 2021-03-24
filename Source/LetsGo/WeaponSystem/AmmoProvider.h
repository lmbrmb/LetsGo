#pragma once

#include "LetsGo/Data/SharedValue.h"

class AmmoProvider : public SharedValue<int>
{
public:
	AmmoProvider(const int min, const int max, const int current, const FName& ammoId);

	const FName& GetAmmoId() const;

private:
	FName _ammoId;
};
