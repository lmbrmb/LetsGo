#pragma once

#include "LetsGo/FloatParameterComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void Kill();

	bool IsAlive() const;
};
