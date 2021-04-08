#pragma once

#include "MappingComponent.h"

#include "AvatarSfxToHealthMapping.generated.h"

/// <summary>
/// Avatar SFX to health mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAvatarSfxToHealthMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
