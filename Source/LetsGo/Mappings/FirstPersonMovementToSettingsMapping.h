#pragma once

#include "MappingComponent.h"

#include "FirstPersonMovementToSettingsMapping.generated.h"

///<summary>
///First person movement to settings mapping
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UFirstPersonMovementToSettingsMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _mouseSensitivityMax = 20.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _cameraPitchSpeedMax = 270;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _actorYawSpeedMax = 270;
};
