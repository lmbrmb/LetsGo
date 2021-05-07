#include "IGun.h"

IAmmoProvider* IGun::GetAmmoProvider() const
{
	return _ammoProvider;
}

IAimProvider* IGun::GetAimProvider() const
{
	return _aimProvider;
}

USceneComponent* IGun::GetShotTraceOrigin() const
{
	return _shotTraceOrigin;
}

bool IGun::IsGunInitialized() const
{
	return _isInitialized;
}

void IGun::InitializeGun(
	IAmmoProvider* ammoProvider,
	IAimProvider* aimProvider,
	USceneComponent* shotTraceOrigin
)
{
	_ammoProvider = ammoProvider;
	_aimProvider = aimProvider;
	_shotTraceOrigin = shotTraceOrigin;
	_isInitialized = true;
	GunInitialized.Broadcast();
}
