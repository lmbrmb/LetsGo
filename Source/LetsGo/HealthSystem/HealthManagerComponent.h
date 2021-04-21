#pragma once

#include "HealthComponent.h"
#include "Components/ActorComponent.h"
#include "LetsGo/Items/HealthItem.h"
#include "LetsGo/Items/IItemProcessor.h"

#include "HealthManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UHealthManagerComponent : public UActorComponent, public IItemProcessor
{
	GENERATED_BODY()

	virtual bool TryProcessItem(Item* item) override;

	bool ProcessHealthItem(HealthItem* healthItem) const;

public:	
	UHealthManagerComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnOverkill();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnHit(const FHitResult& hitResult);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _overkillHealth = -30.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TArray<FName> _itemsCanHealAboveNormal;

	UHealthComponent* _healthComponent;

	void OnHealthChanged(UHealthComponent* healthComponent, const float delta);

	void OnDied();

	void OnInjured();

	bool CanItemHealAboveNormal(HealthItem* healthItem) const;
};
