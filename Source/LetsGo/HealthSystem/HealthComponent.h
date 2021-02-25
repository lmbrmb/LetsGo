#pragma once

#include "LetsGo/FloatParameterComponent.h"
#include "LetsGo/InventorySystem/InventoryItem.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthComponent final : public UFloatParameterComponent
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UHealthComponent, Died, AActor*);

	Died Died;
	
	void OnInventoryItemAdded(InventoryItem* item);

	void Heal(float amount);

	void Injure(float amount);
	
	void Kill();

	bool IsAlive() const;

	bool IsDead() const;

protected:
	virtual void OnChanged() override;
};
