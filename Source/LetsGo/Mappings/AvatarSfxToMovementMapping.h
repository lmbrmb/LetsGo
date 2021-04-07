#pragma once

#include "MappingComponent.h"

#include "AvatarSfxToMovementMapping.generated.h"

/// <summary>
/// Avatar SFX component to Movement base component mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAvatarSfxToMovementMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
