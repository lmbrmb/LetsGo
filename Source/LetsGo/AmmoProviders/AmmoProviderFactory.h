#pragma once
#include "IAmmoProvider.h"

class AmmoProviderFactory
{
public:
	AmmoProviderFactory() = default;

	IAmmoProvider* CreateAmmoProvider(const FName& ammoId, const int max, const int current) const;
	
	IAmmoProvider* CreateInfiniteAmmoProvider(const FName& ammoId) const;
};

Expose_TNameOf(AmmoProviderFactory);
