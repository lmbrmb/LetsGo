#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementInfoProvider.generated.h"

UINTERFACE(MinimalAPI)
class UMovementInfoProvider : public UInterface
{
	GENERATED_BODY()
};

class LETSGO_API IMovementInfoProvider
{
	GENERATED_BODY()

public:
	virtual float GetAbsoluteMovementAmount() = 0;

	virtual bool GetIsInAir() const = 0;
};
