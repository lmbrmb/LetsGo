#pragma once

#include "GunShotComponent.h"
#include "LineShotComponent.generated.h"

///<summary>
///Line shot component. Single shot. Check from start to end.
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent) )
class LETSGO_API ULineShotComponent final : public UGunShotComponent
{
	GENERATED_BODY()

protected:
	virtual void OnShot(USceneComponent* firePivot, USceneComponent* aimProvider) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnShot(FVector startPosition, FVector endPosition);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _distance = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _damage = 50;

	FHitResult _hitResult;
	
	FCollisionQueryParams _collisionQueryParams;
};
