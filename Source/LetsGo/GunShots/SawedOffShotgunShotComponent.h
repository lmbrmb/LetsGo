#pragma once

#include "GunShotComponent.h"
#include "SawedOffShotgunShotComponent.generated.h"

///<summary>
///SawedOffShotgun shot component
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API USawedOffShotgunShotComponent : public UGunShotComponent
{
	GENERATED_BODY()

public:
	virtual void OnShot(USceneComponent* firePivot, USceneComponent* aimProvider) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnShot(FVector startPosition, FRotator rotation);
};
