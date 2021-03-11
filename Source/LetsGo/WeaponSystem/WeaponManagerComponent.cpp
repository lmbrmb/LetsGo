#include "WeaponManagerComponent.h"

#include "Weapon.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Items/Item.h"
#include "LetsGo/Items/AmmoItem.h"
#include "LetsGo/Items/GunItem.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssetUtils.h"
#include "LetsGo/Utils/ActorUtils.h"

#define RETURN_IF_NO_WEAPON \
if (_weaponActor == nullptr) \
{ \
	return; \
} \

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	_itemProcessors.Add( [this](auto item) { return TryProcessItemAsGun(item); });
	_itemProcessors.Add([this](auto item) { return TryProcessItemAsAmmo(item); });

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const gunFactory = diContainer->GetInstance<GunFactory>();
	_gunFactory = &gunFactory.Get();

	auto const ammoItemFactory = diContainer->GetInstance<AmmoItemFactory>();
	_ammoItemFactory = &ammoItemFactory.Get();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StartFire()
{
	RETURN_IF_NO_WEAPON;

	if(_gun)
	{
		_gun->StartFire();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StopFire()
{
	RETURN_IF_NO_WEAPON;

	if(_gun)
	{
		_gun->StopFire();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::Reload()
{
	RETURN_IF_NO_WEAPON;

	if(_gun)
	{
		_gun->Reload();
	}
}

void UWeaponManagerComponent::NextWeapon()
{
	ChangeWeapon(1);
}

void UWeaponManagerComponent::PreviousWeapon()
{
	ChangeWeapon(-1);
}

void UWeaponManagerComponent::ChangeWeapon(const int indexModifier)
{
	if (indexModifier == 0)
	{
		DevLogger::GetLoggingChannel()->Log("ChangeWeapon: index modifier is 0", LogSeverity::Warning);
		return;
	}
	
	auto const weaponsCount = _weaponActors.Num();

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

void UWeaponManagerComponent::SetPlayerId(const int32 playerId)
{
	_playerId = playerId;
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
	
	for (auto weaponActor : _weaponActors)
	{
		weaponActor->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void UWeaponManagerComponent::EquipWeapon(const int weaponIndex)
{
	if(_weaponIndex == weaponIndex)
	{
		return;
	}
	
	if (_weaponActor)
	{
		if(_gun)
		{
			_gun->StopFire();
		}
		
		ActorUtils::SetEnabled(_weaponActor, false);
	}

	_weaponIndex = weaponIndex;
	_weaponActor = _weaponActors[_weaponIndex];

	// Refresh all known weapons
	_gun = Cast<IGun>(_weaponActor);
	
	ActorUtils::SetEnabled(_weaponActor, true);
}

bool UWeaponManagerComponent::TryProcessItem(Item* item)
{
	for (auto itemProcessor : _itemProcessors)
	{
		auto const isProcessed = itemProcessor(item);
		if(isProcessed)
		{
			return true;
		}
	}

	return false;
}

bool UWeaponManagerComponent::TryProcessItemAsGun(Item* item)
{
	auto const gunItem = dynamic_cast<GunItem*>(item);
	if (gunItem == nullptr)
	{
		return false;
	}

	// Can carry only one gun of type
	// Add ammo if already have this gun
	for (auto weaponActor : _weaponActors)
	{
		auto const weapon = Cast<IWeapon>(weaponActor);
		if(weapon->GetId() == gunItem->GetId())
		{
			auto const ammoId = gunItem->GetAmmoId();
			auto const ammoItem = _ammoItemFactory->Get(ammoId);
			return TryProcessItemAsAmmo(ammoItem);
		}
	}

	auto const gun = CreateGun(gunItem);

	if(!gun)
	{
		return false;
	}
	
	auto const weaponIndex = _weaponActors.Add(gun);

	if (_equipWeaponOnPickup)
	{
		EquipWeapon(weaponIndex);
	}
	else
	{
		ActorUtils::SetEnabled(gun, false);
	}
	
	return true;
}

bool UWeaponManagerComponent::TryProcessItemAsAmmo(Item* item)
{
	auto const ammoItem = dynamic_cast<AmmoItem*>(item);
	if (ammoItem == nullptr)
	{
		return false;
	}

	auto ammoProvider = GetAmmoProvider(ammoItem->GetId());

	if (ammoProvider)
	{
		if(ammoProvider->IsMax())
		{
			return false;
		}
		
		auto const quantity = ammoItem->GetQuantity();
		ammoProvider->Add(quantity);
	}
	else
	{
		CreateAmmoProvider(ammoItem);
	}
	
	return true;
}

AmmoProvider* UWeaponManagerComponent::GetAmmoProvider(const FName ammoId)
{
	if (_ammoProviders.Contains(ammoId))
	{
		return _ammoProviders[ammoId];
	}
	
	return nullptr;
}

AmmoProvider* UWeaponManagerComponent::CreateAmmoProvider(const GunItem* gunItem)
{
	auto const ammoId = gunItem->GetAmmoId();
	auto const ammoItem = static_cast<AmmoItem*>(_ammoItemFactory->Get(ammoId));
	auto const maxAmmo = ammoItem->GetMaxQuantity();
	auto const currentAmmo = gunItem->GetInitialAmmoCount();
	auto const ammoProvider = new AmmoProvider(0, maxAmmo, currentAmmo);
	_ammoProviders.Add(ammoId, ammoProvider);
	return ammoProvider;
}

AmmoProvider* UWeaponManagerComponent::CreateAmmoProvider(const AmmoItem* ammoItem)
{
	auto const ammoId = ammoItem->GetId();
	auto const maxAmmo = ammoItem->GetMaxQuantity();
	auto const currentAmmo = ammoItem->GetQuantity();
	auto const ammoProvider = new AmmoProvider(0, maxAmmo, currentAmmo);
	_ammoProviders.Add(ammoId, ammoProvider);
	return ammoProvider;
}

AActor* UWeaponManagerComponent::CreateGun(const GunItem* gunItem)
{
	auto const gunId = gunItem->GetId();
	auto const weaponBlueprint = _gunFactory->GetBlueprint(gunId);
	
	if(!weaponBlueprint)
	{
		return nullptr;
	}
	
	auto const weaponActor = AssetUtils::SpawnBlueprint<AActor>(GetWorld(), GetOwner(), weaponBlueprint);
	if(!weaponActor)
	{
		DevLogger::GetLoggingChannel()->LogValue("Weapon blueprint is not spawned. Gun item id:", gunItem->GetId());
		return nullptr;
	}

	auto const weapon = Cast<IWeapon>(weaponActor);
	if (!weapon)
	{
		DevLogger::GetLoggingChannel()->LogValue( "IWeapon is null. Gun item id:", gunItem->GetId());
		weaponActor->Destroy();
		return nullptr;
	}
	
	weapon->InitializeWeapon(gunId, _playerId);
	
	auto const gun = Cast<IGun>(weapon);

	if (!gun)
	{
		DevLogger::GetLoggingChannel()->LogValue("IGun is null. Gun item id:", gunItem->GetId());
		weaponActor->Destroy();
		return nullptr;
	}
	
	auto const ammoId = gunItem->GetAmmoId();
	auto ammoProvider = GetAmmoProvider(ammoId);

	if (!ammoProvider)
	{
		ammoProvider = CreateAmmoProvider(gunItem);
	}

	gun->InitializeGun(ammoProvider, _aimProvider);

	if (_weaponPivot == nullptr)
	{
		DevLogger::GetLoggingChannel()->Log("Weapon pivot is null", LogSeverity::Warning);
	}
	else
	{
		weaponActor->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	return weaponActor;
}
