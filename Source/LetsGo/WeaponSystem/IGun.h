#pragma once

#include "IWeapon.h"
#include "LetsGo/AmmoProviders/IAmmoProvider.h"
#include "LetsGo/AimProviders/IAimProvider.h"

DECLARE_EVENT_OneParam(IGun, EShotRequested, const USceneComponent* firePivot);

DECLARE_EVENT_TwoParams(IGun, EShotPerformed, const IGun* gun, const bool isAnyBulletDamaged);

DECLARE_EVENT(IGun, EGunInitialized);

DECLARE_EVENT(IGun, EOutOfAmmo);

/// <summary>
/// [Interface] Gun contract
/// </summary>
class IGun : public IWeapon
{
public:
	EShotRequested ShotRequested;

	EShotPerformed ShotPerformed;

	EGunInitialized GunInitialized;

	EOutOfAmmo OutOfAmmo;

	IAmmoProvider* GetAmmoProvider() const;

	IAimProvider* GetAimProvider() const;

	bool IsGunInitialized() const;

	void InitializeGun(IAmmoProvider* ammoProvider, IAimProvider* aimProvider);

	virtual void OnShotPerformed(const bool isAnyBulletDamaged) = 0;

	virtual void OnBulletTraced(const bool isDamaged, const FHitResult& hitResult) = 0;

	virtual void StartFire() = 0;

	virtual void StopFire() = 0;

	virtual void Reload() = 0;

	virtual bool IsEnoughAmmoForShot() const = 0;

protected:
	virtual ~IGun(){};

private:
	int32 _instigatorId = MIN_int32;

	IAmmoProvider* _ammoProvider = nullptr;

	IAimProvider* _aimProvider = nullptr;

	bool _isInitialized = false;
};
