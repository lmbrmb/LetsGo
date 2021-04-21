#pragma once

#include "LetsGo/Data/Damage.h"
#include "LetsGo/Data/FloatParameterComponent.h"

#include "HealthComponent.generated.h"

class UHealthComponent;

DECLARE_EVENT_TwoParams(UHealthComponent, FDied, UHealthComponent* healthComponent, const float delta);

DECLARE_EVENT_TwoParams(UHealthComponent, FHealthChanged, UHealthComponent* healthComponent, const float delta);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	FDied Died;

	FHealthChanged HealthChanged;

	bool TryHeal(const float healAmount);

	bool TryInjure(const Damage& damage);

	void Kill();

	bool IsAlive() const;

	bool IsDead() const;

	bool IsFullHealth() const;

	float GetMaxNormalHealth() const;

	const Damage& GetLastDamage() const;

protected:
	virtual void OnChanged(float delta) override;

	virtual void Init() override;

	virtual void BeginDestroy() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float _maxNormalHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Decrease Health Over Time")
	float _decreaseHealthInterval = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Decrease Health Over Time")
	float _decreaseHealthAmount = 0;

	Damage _lastDamage;

	FTimerHandle _decreaseHealthTimerHandle;

	void DecreaseHealthOnTimer();
};
