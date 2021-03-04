#pragma once

#include "LetsGo/FloatParameterComponent.h"

#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UHealthComponent, Died, AActor*);

	Died Died;

	void Heal(float healAmount);

	void Injure(float damageAmount);
	
	void Kill();

	bool IsAlive() const;

	bool IsDead() const;

	bool IsFullHealth() const;

protected:
	virtual void OnChanged() override;
};
