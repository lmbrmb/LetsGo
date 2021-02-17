#include "WeaponManagerComponent.h"
#include "LetsGo/GameModes/LetsGoGameModeBase.h"
#include "LetsGo/InventorySystem/InventoryItem.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/Utils/AssetUtils.h"
#include "LetsGo/Utils/ActorUtils.h"

#define RETURN_IF_NO_WEAPON \
if (_currentWeapon == nullptr) \
{ \
	return; \
} \

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	auto const gameModeBase = dynamic_cast<ALetsGoGameModeBase*>(GetWorld()->GetAuthGameMode());
	auto const diContainer = gameModeBase->GetDiContainer();
	auto const weaponInventoryItemFactory = diContainer->GetInstance<WeaponFactory>();
	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_weaponFactory = &weaponInventoryItemFactory.Get();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StartFire()
{
	RETURN_IF_NO_WEAPON;

	_currentWeapon->StartFire();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StopFire()
{
	RETURN_IF_NO_WEAPON;

	_currentWeapon->StopFire();
}

void UWeaponManagerComponent::Reload()
{
	RETURN_IF_NO_WEAPON;

	_currentWeapon->Reload();
}

void UWeaponManagerComponent::NextWeapon()
{
	ChangeWeapon(1);
}

void UWeaponManagerComponent::PreviousWeapon()
{
	ChangeWeapon(-1);
}

void UWeaponManagerComponent::ChangeWeapon(int indexModifier)
{
	if (indexModifier == 0)
	{
		DevLogger::GetLoggingChannel()->Log("ChangeWeapon: index modifier is 0", LogSeverity::Warning);
		return;
	}
	
	auto const weaponsCount = _weapons.Num();

	if (weaponsCount == 0)
	{
		return;
	}

	auto nextIndex = _currentWeaponIndex + indexModifier;

	if (nextIndex >= weaponsCount)
	{
		nextIndex = 0;
	}
	else if (nextIndex < 0)
	{
		nextIndex = weaponsCount - 1;
	}

	EquipWeapon(nextIndex);
}

void UWeaponManagerComponent::SetWeaponPivot(USceneComponent* weaponPivot)
{
	_weaponPivot = weaponPivot;
	if(_currentWeapon)
	{
		for(auto weapon : _weapons)
		{
			weapon->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void UWeaponManagerComponent::EquipWeapon(int weaponIndex)
{
	if (_currentWeapon)
	{
		ActorUtils::SetEnabled(_currentWeapon, false);
	}

	_currentWeaponIndex = weaponIndex;
	_currentWeapon = _weapons[_currentWeaponIndex];

	ActorUtils::SetEnabled(_currentWeapon, true);
}

void UWeaponManagerComponent::OnInventoryItemAdded(InventoryItem* item)
{
	auto const weaponBlueprint = _weaponFactory->GetBlueprint(item->GetId());
	auto const weapon = AssetUtils::SpawnBlueprint<AWeaponBase>(GetOwner(), weaponBlueprint);

	if(_weaponPivot == nullptr)
	{
		DevLogger::GetLoggingChannel()->Log("Weapon pivot is null", LogSeverity::Warning);
	}
	else
	{
		weapon->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
	}

	auto const weaponIndex = _weapons.Add(weapon);
	
	if (_equipWeaponOnPickup)
	{
		EquipWeapon(weaponIndex);
	}
	else
	{
		ActorUtils::SetEnabled(weapon, false);
	}
}

void UWeaponManagerComponent::OnInventoryItemRemoved(InventoryItem* item)
{
	DevLogger::GetLoggingChannel()->Log("WeaponManager. OnInventoryItemRemoved");
}
