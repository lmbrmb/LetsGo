#pragma once

#include "LetsGo/Items/Item.h"
#include "LetsGo/Items/IItemFactory.h"

#include "PickupManagerComponent.generated.h"

///<summary>
/// Pickup manager component
///</summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UPickupManagerComponent final : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPickupManagerComponent();
	
	DECLARE_EVENT_OneParam(UPickupManagerComponent, EItemPickedUp, Item* item);
	
	UFUNCTION(BlueprintCallable)
	bool TryPickUpItem(FName itemId);
	
	/// <summary>
	/// [Code] Item added event
	/// </summary>
	EItemPickedUp ItemPickedUp;
	
protected:
	virtual void BeginPlay() override;
	
private:
	TArray<IItemFactory*> _itemFactories;
};
