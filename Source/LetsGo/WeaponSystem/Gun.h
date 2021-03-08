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
	DECLARE_EVENT_ThreeParams(
	IGun,
		EShotPerformed,
		const FGuid& instigatorId,
		const USceneComponent*,
		const USceneComponent*
		);
	
	EShotPerformed ShotPerformed;
	
	virtual void Init(const FGuid instigatorId, AmmoProvider* ammoProvider, USceneComponent* aimProvider ) = 0;

	virtual void StartFire() = 0;

	virtual void StopFire() = 0;

	virtual void Reload() = 0;
};
