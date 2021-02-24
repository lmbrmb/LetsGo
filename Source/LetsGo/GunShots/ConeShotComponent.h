#pragma once

#include "CoreMinimal.h"
#include "GunShotComponent.h"
#include "ConeShotComponent.generated.h"

///<summary>
///Cone shot component. Cone dispersion. N bullets.
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UConeShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

public:
	virtual void OnShot(USceneComponent* firePivot, USceneComponent* aimProvider) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnShot(FVector startPosition, FRotator rotation);
};
