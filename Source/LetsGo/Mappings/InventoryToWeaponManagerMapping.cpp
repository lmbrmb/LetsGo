#include "InventoryToWeaponManagerMapping.h"

UInventoryToWeaponManagerMapping::UInventoryToWeaponManagerMapping()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryToWeaponManagerMapping::BeginPlay()
{
	Super::BeginPlay();
	
	auto const owner = GetOwner();
	auto const inventoryComponent = owner->FindComponentByClass<UInventoryComponent>();
	auto const weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	inventoryComponent->ItemAdded.AddUObject(weaponManagerComponent, &UWeaponManagerComponent::OnInventoryItemAdded);
	inventoryComponent->ItemRemoved.AddUObject(weaponManagerComponent, &UWeaponManagerComponent::OnInventoryItemAdded);

	// Mapping is created, no need in this component anymore
	DestroyComponent();
}
