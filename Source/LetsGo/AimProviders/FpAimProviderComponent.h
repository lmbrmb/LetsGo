#pragma once

#include "AimProviderComponent.h"

#include "FpAimProviderComponent.generated.h"

/// <summary>
/// First person aim provider component
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UFpAimProviderComponent : public UAimProviderComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetAimPivot(USceneComponent* aimPivot);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _aimRange = 2000.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TEnumAsByte<ECollisionChannel> _collisionChannel;
};
