#pragma once

#include "HealthComponent.h"
#include "Components/ActorComponent.h"
#include "LetsGo/Items/HealthItem.h"
#include "LetsGo/Items/IItemProcessor.h"

#include "HealthManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthManagerComponent : public UActorComponent, public IItemProcessor
{
	GENERATED_BODY()

	virtual bool TryProcessItem(Item* item) override;

	bool ProcessHealthItem(HealthItem* healthItem) const;
	
public:	
	UHealthManagerComponent();

protected:
	virtual void BeginPlay() override;

private:
	UHealthComponent* _healthComponent;
	
	void OnDied(const UHealthComponent*, float delta) const;

	static bool CanItemHealAboveNormal(HealthItem* healthItem);
};
