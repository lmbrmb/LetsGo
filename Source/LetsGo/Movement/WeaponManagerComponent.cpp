#include "WeaponManagerComponent.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/InventorySystem/InventoryItem.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"
#include <LetsGo/Utils/AssetUtils.h>

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	auto const actor = GetOwner();
	
	auto const inputComponent = actor->InputComponent;
	if(inputComponent)
	{
		MapPlayerInput(inputComponent);
	}
}

void UWeaponManagerComponent::MapPlayerInput(UInputComponent* playerInputComponent)
{
	playerInputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed, this, &UWeaponManagerComponent::PrimaryFire);
}

void UWeaponManagerComponent::PrimaryFire()
{
	if(!_currentWeapon)
	{
		return;
	}

	_currentWeapon->Fire();
}

void UWeaponManagerComponent::EquipWeapon(AWeaponBase* weapon)
{
	_currentWeapon = weapon;
}

void UWeaponManagerComponent::OnInventoryItemAdded(InventoryItem* item)
{
	auto const weaponBlueprint = _weaponFactory.GetBlueprint(item->GetId());
	auto const weapon = AssetUtils::SpawnBlueprint<AWeaponBase>(GetOwner(), weaponBlueprint);
	weapon->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
	_weapons.Add(weapon);
	
	if (_equipWeaponOnPickup)
	{
		EquipWeapon(weapon);
	}
}

void UWeaponManagerComponent::OnInventoryItemRemoved(InventoryItem* item)
{
	DevLogger::GetLoggingChannel()->Log("WeaponManager. OnInventoryItemRemoved");
}
