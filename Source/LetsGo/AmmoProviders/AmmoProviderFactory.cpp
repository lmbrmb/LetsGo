#include "AmmoProviderFactory.h"

#include "AmmoProvider.h"
#include "InfiniteAmmoProvider.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAmmoProvider* AmmoProviderFactory::CreateAmmoProvider(
	const FName& ammoId,
	const int max,
	const int current
) const
{
	return new AmmoProvider(ammoId, max, current);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
IAmmoProvider* AmmoProviderFactory::CreateInfiniteAmmoProvider(const FName& ammoId) const
{
	return new InfiniteAmmoProvider(ammoId);
}
