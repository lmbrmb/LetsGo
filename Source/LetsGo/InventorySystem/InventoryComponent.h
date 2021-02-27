#pragma once

#include "InventoryItem.h"
#include "InventoryItemFactory.h"
#include "ItemConversionFactory.h"

#include "InventoryComponent.generated.h"

///<summary>
/// Inventory component
///</summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()
	
public:
	DECLARE_EVENT_OneParam(UInventoryComponent, EItemAdded, InventoryItem* item);
	
	DECLARE_EVENT_OneParam(UInventoryComponent, EItemRemoved, InventoryItem* item);
	
	UInventoryComponent();

	InventoryItem* GetInventoryItem(FName itemId) const;
	
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(FName itemId);
	
	UFUNCTION(BlueprintCallable)
	bool TryRemoveItem(FName itemId);

	/// <summary>
	/// [Code] Item added event
	/// </summary>
	EItemAdded ItemAdded;
	
	/// <summary>
	/// [Code] Item removed event
	/// </summary>
	EItemRemoved ItemRemoved;

protected:
	virtual void BeginPlay() override;
	
private:
	TArray<InventoryItem*> _inventoryItems;

	TArray<InventoryItemFactory*> _inventoryItemFactories;

	ItemConversionFactory* _itemConversionFactory;
};
