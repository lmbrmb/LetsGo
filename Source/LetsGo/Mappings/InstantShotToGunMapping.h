#pragma once

#include "GunShotToGunMapping.h"

#include "InstantShotToGunMapping.generated.h"

/// <summary>
/// Instant shot to gun mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInstantShotToGunMapping : public UGunShotToGunMapping
{
	GENERATED_BODY()

protected:
	virtual void Bind() const override;
};
