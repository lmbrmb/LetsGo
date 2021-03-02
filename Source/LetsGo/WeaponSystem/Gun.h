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
	DECLARE_EVENT_TwoParams(IGun, EShotPerformed, USceneComponent*, USceneComponent*);
	EShotPerformed ShotPerformed;
	
	virtual void Init(AmmoProvider* ammoProvider, USceneComponent* aimProvider ) = 0;

	virtual void StartFire() = 0;

	virtual void StopFire() = 0;

	virtual void Reload() = 0;
};
