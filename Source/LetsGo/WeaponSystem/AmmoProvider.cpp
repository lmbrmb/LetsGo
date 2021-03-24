#include "AmmoProvider.h"

AmmoProvider::AmmoProvider(const int min, const int max, const int current, const FName& ammoId)
	: SharedValue<int>(min, max, current),
	_ammoId(ammoId)
{
}

const FName& AmmoProvider::GetAmmoId() const
{
	return _ammoId;
}
