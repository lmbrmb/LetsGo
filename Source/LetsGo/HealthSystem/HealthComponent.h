#pragma once

#include "Damage.h"
#include "LetsGo/FloatParameterComponent.h"

#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChangedDelegate);

	UPROPERTY(BlueprintAssignable)
	FHealthChangedDelegate BpHealthChanged;
	
	DECLARE_EVENT_TwoParams(UHealthComponent, FDied, const UHealthComponent*, float delta);

	DECLARE_EVENT_TwoParams(UHealthComponent, FHealthChanged, const UHealthComponent*, float delta);
	
	FDied Died;

	FHealthChanged HealthChanged;
	
	void Heal(const float healAmount);

	void Injure(const Damage damage);
	
	void Kill();

	bool IsAlive() const;

	bool IsDead() const;

	bool IsFullHealth() const;

	Damage GetLastDamage() const;
	
protected:
	virtual void OnChanged(float delta) override;

private:
	Damage _lastDamage = Damage(MIN_int32, FName(""), 0);
};
