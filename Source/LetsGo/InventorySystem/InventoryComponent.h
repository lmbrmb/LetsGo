#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFactory.h"
#include "Delegates/Delegate.h"
#include "InventoryComponent.generated.h"

///<summary>
/// 
///</summary>
UCLASS( ClassGroup=(_Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAdded, FName, itemId);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemoved, FName, itemId);
	
	DECLARE_EVENT_OneParam(UInventoryComponent, EItemAdded, FName itemId);
	
	DECLARE_EVENT_OneParam(UInventoryComponent, EItemRemoved, FName itemId);
	
	UInventoryComponent();
	
	virtual void BeginPlay() override;

	InventoryItem* GetInventoryItem() const;
	
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(FName itemId);
	
	UFUNCTION(BlueprintCallable)
	bool TryRemoveItem(FName itemId);

	/// <summary>
	/// [BP] Item added delegate
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FItemAdded ItemAddedDelegate;

	/// <summary>
	/// [BP] Item removed delegate
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FItemRemoved ItemRemovedDelegate;

	/// <summary>
	/// [Code] Item added event
	/// </summary>
	EItemAdded ItemAdded;
	
	/// <summary>
	/// [Code] Item removed event
	/// </summary>
	EItemRemoved ItemRemoved;

private:
	InventoryItem* CreateOnlyOneItem(FName itemId);
	
	TArray<InventoryItem*> _inventoryItems;

	TArray<InventoryItemFactory*> _inventoryItemFactories;
};
