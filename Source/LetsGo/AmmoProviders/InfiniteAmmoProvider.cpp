#include "InfiniteAmmoProvider.h"

InfiniteAmmoProvider::InfiniteAmmoProvider(const FName& ammoId)
	: _ammoId(ammoId)
{
}

const FName& InfiniteAmmoProvider::GetAmmoId() const
{
	return _ammoId;
}

int InfiniteAmmoProvider::GetCurrent() const
{
	return INT_MAX;
}

int InfiniteAmmoProvider::GetMin() const
{
	return INT_MAX;
}

int InfiniteAmmoProvider::GetMax() const
{
	return INT_MAX;
}

bool InfiniteAmmoProvider::IsMin() const
{
	return false;
}

bool InfiniteAmmoProvider::IsMax() const
{
	return true;
}

int InfiniteAmmoProvider::Add(const int amount)
{
	return 0;
}

int InfiniteAmmoProvider::Remove(const int amount)
{
	return 0;
}
