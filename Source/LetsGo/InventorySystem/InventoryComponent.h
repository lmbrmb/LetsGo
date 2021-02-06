#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "InventoryItemFactory.h"
#include "InventoryComponent.generated.h"

///<summary>
/// Inventory component
///</summary>
UCLASS( ClassGroup=(_Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedSignature, FName, itemId);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedSignature, FName, itemId);

	DECLARE_EVENT_OneParam(UInventoryComponent, EItemAdded, InventoryItem* item);
	
	DECLARE_EVENT_OneParam(UInventoryComponent, EItemRemoved, InventoryItem* item);
	
	UInventoryComponent();
	
	virtual void BeginPlay() override;

	InventoryItem* GetInventoryItem(FName itemId) const;
	
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(FName itemId);
	
	UFUNCTION(BlueprintCallable)
	bool TryRemoveItem(FName itemId);

	/// <summary>
	/// [BP] Item added delegate
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FItemAddedSignature ItemAddedD;

	/// <summary>
	/// [BP] Item removed delegate
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FItemRemovedSignature ItemRemovedD;

	/// <summary>
	/// [Code] Item added event
	/// </summary>
	EItemAdded ItemAdded;
	
	/// <summary>
	/// [Code] Item removed event
	/// </summary>
	EItemRemoved ItemRemoved;

private:
	TArray<InventoryItem*> _inventoryItems;

	TArray<InventoryItemFactory*> _inventoryItemFactories;
};
