#include "WeaponManagerComponent.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/InventorySystem/InventoryItem.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/WeaponSystem/WeaponFactory.h"
#include "LetsGo/Utils/AssetUtils.h"
#include "LetsGo/Utils/ActorUtils.h"

const float MIN_ABS_CHANGE_WEAPON = 0.1f;

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
	playerInputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed, this, &UWeaponManagerComponent::StartFire);
	playerInputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Released, this, &UWeaponManagerComponent::StopFire);
	playerInputComponent->BindAction(InputConstant::ActionReload, EInputEvent::IE_Pressed, this, &UWeaponManagerComponent::Reload);
	playerInputComponent->BindAction(InputConstant::ActionPreviousWeapon, EInputEvent::IE_Pressed, this, &UWeaponManagerComponent::PreviousWeapon);
	playerInputComponent->BindAction(InputConstant::ActionNextWeapon, EInputEvent::IE_Pressed, this, &UWeaponManagerComponent::NextWeapon);
	playerInputComponent->BindAxis(InputConstant::AxisChangeWeapon, this, &UWeaponManagerComponent::ChangeWeapon);
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
	DevLogger::GetLoggingChannel()->Log("NextWeapon");
}

void UWeaponManagerComponent::PreviousWeapon()
{
	DevLogger::GetLoggingChannel()->Log("PreviousWeapon");
}

void UWeaponManagerComponent::ChangeWeapon(float value)
{
	if(FMath::Abs(value) < MIN_ABS_CHANGE_WEAPON)
	{
		return;
	}
	
	DevLogger::GetLoggingChannel()->LogValue("ChangeWeapon: ", value);
}

void UWeaponManagerComponent::EquipWeapon(AWeaponBase* weapon)
{
	if (_currentWeapon)
	{
		ActorUtils::SetEnabled(_currentWeapon, false);
	}
	
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
