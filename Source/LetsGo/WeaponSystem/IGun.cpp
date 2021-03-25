#include "IGun.h"

AmmoProvider* IGun::GetAmmoProvider() const
{
	return _ammoProvider;
}

IAimProvider* IGun::GetAimProvider() const
{
	return _aimProvider;
}

bool IGun::IsGunInitialized() const
{
	return _isInitialized;
}

void IGun::InitializeGun(AmmoProvider* ammoProvider, IAimProvider* aimProvider)
{
	_ammoProvider = ammoProvider;
	_aimProvider = aimProvider;
	_isInitialized = true;
	GunInitialized.Broadcast();
}
