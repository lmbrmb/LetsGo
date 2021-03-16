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
	virtual void OnShotRequested(const USceneComponent* firePivot) override;
};
