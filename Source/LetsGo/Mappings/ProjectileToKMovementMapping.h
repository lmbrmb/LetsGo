#pragma once

#include "MappingComponent.h"
#include "ProjectileToKMovementMapping.generated.h"

/// <summary>
/// Projectile to kinematic movement component mapping 
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UProjectileToKMovementMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
