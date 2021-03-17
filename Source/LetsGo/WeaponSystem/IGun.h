#pragma once

#include "IWeapon.h"
#include "AmmoProvider.h"

DECLARE_EVENT_OneParam(IGun, EShotRequested, const USceneComponent* firePivot);

DECLARE_EVENT_TwoParams(IGun, EShotPerformed, const IGun* gun, const bool isAnyBulletDamaged);

DECLARE_EVENT_OneParam(IGun, EGunInitialized, IGun*);

/// <summary>
/// [Interface] Gun contract
/// </summary>
class IGun : public IWeapon
{
public:
	EShotRequested ShotRequested;

	EShotPerformed ShotPerformed;
	
	EGunInitialized GunInitialized;

	AmmoProvider* GetAmmoProvider() const;

	USceneComponent* GetAimProvider() const;
	
	bool IsGunInitialized() const;
	
	void InitializeGun(AmmoProvider* ammoProvider, USceneComponent* aimProvider);

	virtual void OnShotPerformed(const bool isAnyBulletDamaged) = 0;
	
	virtual void StartFire() = 0;

	virtual void StopFire() = 0;

	virtual void Reload() = 0;

protected:
	virtual ~IGun(){};

private:
	int32 _instigatorId = MIN_int32;

	AmmoProvider* _ammoProvider = nullptr;

	USceneComponent* _aimProvider = nullptr;

	bool _isInitialized = false;
};
