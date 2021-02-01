#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(_Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void AddItem(FName itemId);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(FName itemId);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAdded, FName, itemId);
	FItemAdded ItemAddedDelegate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemoved, FName, itemId);
	FItemRemoved ItemRemovedDelegate;
	
	DECLARE_EVENT_OneParam(UInventoryComponent, EItemAdded, FName itemId);
	EItemAdded ItemAdded;

	DECLARE_EVENT_OneParam(UInventoryComponent, EItemRemoved, FName itemId);
	EItemRemoved ItemRemoved;

private:
	TArray<FName> _itemIds;
};
