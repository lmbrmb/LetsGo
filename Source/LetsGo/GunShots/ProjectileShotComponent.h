#pragma once

#include "CoreMinimal.h"
#include "GunShotComponent.h"

#include "ProjectileShotComponent.generated.h"

///<summary>
///Projectile shot component.
///</summary>
UCLASS()
class LETSGO_API UProjectileShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

public:
	virtual void OnShot(
		const int32 instigatorId,
		const USceneComponent* firePivot,
		const USceneComponent* aimProvider
	) override;
};
