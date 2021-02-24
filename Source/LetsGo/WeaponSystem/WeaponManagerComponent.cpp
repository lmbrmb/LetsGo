#include "WeaponManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/InventorySystem/InventoryItem.h"
#include "LetsGo/InventorySystem/WeaponItem.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/Utils/AssetUtils.h"
#include "LetsGo/Utils/ActorUtils.h"

#define RETURN_IF_NO_WEAPON \
if (_weapon == nullptr) \
{ \
	return; \
} \

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	auto const gameModeBase = dynamic_cast<AMatchGameMode*>(GetWorld()->GetAuthGameMode());
	auto const diContainer = gameModeBase->GetDiContainer();
	auto const weaponInventoryItemFactory = diContainer->GetInstance<WeaponFactory>();
	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_weaponFactory = &weaponInventoryItemFactory.Get();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StartFire()
{
	RETURN_IF_NO_WEAPON;

	_weapon->StartFire();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StopFire()
{
	RETURN_IF_NO_WEAPON;

	_weapon->StopFire();
}

void UWeaponManagerComponent::Reload()
{
	RETURN_IF_NO_WEAPON;

	_weapon->Reload();
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

	auto nextIndex = _weaponIndex + indexModifier;

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

void UWeaponManagerComponent::AddWeaponPivot(USceneComponent* weaponPivot)
{
	if(weaponPivot == nullptr)
	{
		return;
	}
	
	_weaponPivots.Add(weaponPivot);

	if (_weaponPivotIndex != UNDEFINED_INDEX)
	{
		return;
	}

	ChangeWeaponPivot();
}

void UWeaponManagerComponent::SetAimProvider(USceneComponent* aimProvider)
{
	_aimProvider = aimProvider;
}

void UWeaponManagerComponent::ChangeWeaponPivot()
{
	auto const weaponPivotsCount = _weaponPivots.Num();
	if(weaponPivotsCount == 0)
	{
		return;
	}
	
	auto nextIndex = _weaponPivotIndex + 1;
	if (nextIndex >= weaponPivotsCount)
	{
		nextIndex = 0;
	}

	if(_weaponPivotIndex == nextIndex)
	{
		return;
	}
	
	_weaponPivotIndex = nextIndex;
	_weaponPivot = _weaponPivots[_weaponPivotIndex];
	
	for (auto weapon : _weapons)
	{
		weapon->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void UWeaponManagerComponent::EquipWeapon(int weaponIndex)
{
	if(_weaponIndex == weaponIndex)
	{
		return;
	}
	
	if (_weapon)
	{
		_weapon->StopFire();
		ActorUtils::SetEnabled(_weapon, false);
	}

	_weaponIndex = weaponIndex;
	_weapon = _weapons[_weaponIndex];

	ActorUtils::SetEnabled(_weapon, true);
}

void UWeaponManagerComponent::OnInventoryItemAdded(InventoryItem* item)
{
	auto const weaponItem = dynamic_cast<WeaponItem*>(item);
	if(weaponItem == nullptr)
	{
		return;
	}
	
	auto const weaponBlueprint = _weaponFactory->GetBlueprint(item->GetId());
	auto const weapon = AssetUtils::SpawnBlueprint<AWeaponBase>(GetWorld(), GetOwner(), weaponBlueprint);
	weapon->SetAimProvider(_aimProvider);
	
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
