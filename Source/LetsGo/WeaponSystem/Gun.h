#pragma once

#include "AmmoProvider.h"
#include "UObject/Interface.h"

#include "Gun.generated.h"

/// <summary>
/// [Generated] Must-have class for IGun
/// </summary>
UINTERFACE(MinimalAPI)
class UGun : public UInterface
{
	GENERATED_BODY()
};

/// <summary>
/// [Abstract] Gun contract
/// </summary>
class LETSGO_API IGun
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(
		IGun,
		EShotPerformed,
		const USceneComponent* firePivot
		);

	DECLARE_EVENT_OneParam(
		IGun,
		EGunInitialized,
		IGun*
		);
	
	EShotPerformed ShotPerformed;

	EGunInitialized GunInitialized;

	AmmoProvider* GetAmmoProvider() const;

	USceneComponent* GetAimProvider() const;
	
	bool IsGunInitialized() const;
	
	void InitializeGun(AmmoProvider* ammoProvider, USceneComponent* aimProvider);

	virtual void StartFire() = 0;

	virtual void StopFire() = 0;

	virtual void Reload() = 0;

private:
	int32 _instigatorId = MIN_int32;

	AmmoProvider* _ammoProvider = nullptr;

	USceneComponent* _aimProvider = nullptr;

	bool _isInitialized = false;
};
