#pragma once

#include "LetsGo/FloatParameterComponent.h"
#include "LetsGo/Items/Item.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UHealthComponent, Died, AActor*);

	Died Died;
	
	void OnItemPickedUp(Item* item);

	void Heal(float healAmount);

	void Injure(float damageAmount);
	
	void Kill();

	bool IsAlive() const;

	bool IsDead() const;

protected:
	virtual void OnChanged() override;
};
