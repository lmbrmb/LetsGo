#pragma once

#include "GunShotComponent.h"
#include "RailgunShotComponent.generated.h"

///<summary>
///Railgun shot component
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent) )
class LETSGO_API URailgunShotComponent final : public UGunShotComponent
{
	GENERATED_BODY()

protected:
	virtual void OnShot(USceneComponent* firePivot, USceneComponent* aimProvider) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnShot(FVector startPosition, FVector endPosition);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _rayDistance = 500;
};
