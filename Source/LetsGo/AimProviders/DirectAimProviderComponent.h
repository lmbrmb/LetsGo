#pragma once

#include "AimProviderComponent.h"
#include "DirectAimProviderComponent.generated.h"

///<summary>
/// Direct aim provider component
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UDirectAimProviderComponent : public UAimProviderComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
