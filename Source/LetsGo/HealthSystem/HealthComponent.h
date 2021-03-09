#pragma once

#include "Damage.h"
#include "LetsGo/FloatParameterComponent.h"

#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UHealthComponent, Died, AActor*);

	Died Died;

	void Heal(const float healAmount);

	void Injure(const Damage damage);
	
	void Kill();

	bool IsAlive() const;

	bool IsDead() const;

	bool IsFullHealth() const;

protected:
	virtual void OnChanged() override;

private:
	Damage _lastDamage = Damage(FGuid(0,0,0,0), 0);
};
