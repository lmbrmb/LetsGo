#include "WeaponManagerComponent.h"

#include "IWeapon.h"
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
	Super::BeginPlay();

	_itemProcessors.Add( [this](auto item) { return this->TryProcessItemAsGun(item); });
	_itemProcessors.Add([this](auto item) { return this->TryProcessItemAsAmmo(item); });

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const gunFactory = diContainer->GetInstance<GunFactory>();
	_gunFactory = &gunFactory.Get();

	auto const ammoItemFactory = diContainer->GetInstance<AmmoItemFactory>();
	_ammoItemFactory = &ammoItemFactory.Get();

	auto const gunItemFactory = diContainer->GetInstance<GunItemFactory>();
	_gunItemFactory = &gunItemFactory.Get();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StartFire()
{
	_isFireStarted = true;
	StartWeaponFire();
}

void UWeaponManagerComponent::StartWeaponFire() const
{
	RETURN_IF_NO_WEAPON;

	if (_gun)
	{
		_gun->StartFire();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWeaponManagerComponent::StopFire()
{
	_isFireStarted = false;
	StopWeaponFire();
}

void UWeaponManagerComponent::StopWeaponFire() const
{
	RETURN_IF_NO_WEAPON;

	if (_gun)
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

void UWeaponManagerComponent::HolsterWeapon()
{
	EquipWeapon(-1);
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

	if (_weaponIndex == nextIndex)
	{
		return;
	}
	
	_nextWeaponIndex = nextIndex;

	if(_changeWeaponTimerHandle.IsValid())
	{
		return;
	}

	if(_weapon)
	{
		if(!_weapon->IsRequestReady())
		{
			_changeWeaponTimerHandle = _weapon->RequestReady.AddUObject(this, &UWeaponManagerComponent::ChangeWeaponOnRequestReady);
			return;
		}
	}

	ChangeWeaponOnRequestReady();
}

void UWeaponManagerComponent::ChangeWeaponOnRequestReady()
{
	if(_changeWeaponTimerHandle.IsValid())
	{
		AssertIsNotNull(_weapon);
		_weapon->RequestReady.Remove(_changeWeaponTimerHandle);
		_changeWeaponTimerHandle.Reset();
	}

	EquipWeapon(_nextWeaponIndex);
	BpOnWeaponChanged();
}

void UWeaponManagerComponent::AddWeaponPivot(USceneComponent* weaponPivot)
{
	if(weaponPivot == nullptr)
	{
		return;
	}
	
	_weaponPivots.Add(weaponPivot);

	if(_weaponPivot)
	{
		return;
	}

	ChangeWeaponPivot();
	OnPartialInitialization();
}

void UWeaponManagerComponent::SetAimProvider(IAimProvider* aimProvider)
{
	_aimProvider = aimProvider;
	OnPartialInitialization();
}

void UWeaponManagerComponent::SetOwnerSkeletalMeshComponent(USkeletalMeshComponent* ownerSkeletalMeshComponent)
{
	_ownerSkeletalMeshComponent = ownerSkeletalMeshComponent;
}

void UWeaponManagerComponent::SetPlayerId(const PlayerId& playerId)
{
	_playerId = playerId;
	OnPartialInitialization();
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
		AttachWeapon(weaponActor);
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
		StopWeaponFire();
		ActorUtils::SetEnabled(_weaponActor, false);
	}
	
	_weaponIndex = weaponIndex;

	auto const noWeapon = _weaponIndex < 0;

	// Refresh all known weapons
	_weaponActor = noWeapon ? nullptr : _weaponActors[_weaponIndex];
	_weapon = noWeapon ? nullptr : _weapons[_weaponIndex];
	_gun = noWeapon ? nullptr : dynamic_cast<IGun*>(_weaponActor);

	if(_weaponActor)
	{
		ActorUtils::SetEnabled(_weaponActor, true);
	}
	
	WeaponEquipped.Broadcast();
	
	if (_isFireStarted)
	{
		StartWeaponFire();
	}
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
	auto const gunId = gunItem->GetId();
	auto const ammoId = gunItem->GetAmmoId();
	auto haveThisGun = false;
	auto haveGunWithSameAmmo = false;
	
	for (auto weapon : _weapons)
	{
		if(weapon->GetWeaponId().GetId() == gunId)
		{
			haveThisGun = true;
		}
		
		auto const gun = dynamic_cast<const IGun*>(weapon);
		if(gun && gun->GetAmmoProvider()->GetAmmoId() == ammoId)
		{
			haveGunWithSameAmmo = true;
		}
	}

	if (haveThisGun || haveGunWithSameAmmo)
	{
		auto const ammoItem = _ammoItemFactory->Get(ammoId);
		TryProcessItemAsAmmo(ammoItem);
	}

	if(haveThisGun)
	{
		return true;
	}
	
	auto const gunActor = CreateGun(gunItem);

	if(!gunActor)
	{
		return false;
	}

	// No null check, CreateGun already checked that created gun is IGun
	auto const noWeapons = _weapons.Num() <= 0;
	auto const gun = dynamic_cast<IGun*>(gunActor);
	_weapons.Add(gun);
	
	auto const weaponIndex = _weaponActors.Add(gunActor);

	if (noWeapons || _shouldEquipWeaponOnPickup)
	{
		EquipWeapon(weaponIndex);
	}
	else
	{
		ActorUtils::SetEnabled(gunActor, false);
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

	auto const ammoItemId = ammoItem->GetId();
	auto ammoProvider = GetAmmoProvider(ammoItemId);

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

IWeapon* UWeaponManagerComponent::GetCurrentWeapon() const
{
	return _weapon;
}

IGun* UWeaponManagerComponent::GetCurrentGun() const
{
	return _gun;
}

AmmoProvider* UWeaponManagerComponent::CreateAmmoProvider(const GunItem* gunItem)
{
	auto const ammoId = gunItem->GetAmmoId();
	auto const ammoItem = _ammoItemFactory->Get(ammoId);
	auto const maxAmmo = ammoItem->GetMaxQuantity();
	auto const currentAmmo = gunItem->GetInitialAmmoCount();
	auto const ammoProvider = new AmmoProvider(0, maxAmmo, currentAmmo, ammoId);
	_ammoProviders.Add(ammoId, ammoProvider);
	return ammoProvider;
}

AmmoProvider* UWeaponManagerComponent::CreateAmmoProvider(const AmmoItem* ammoItem)
{
	auto const ammoId = ammoItem->GetId();
	auto const maxAmmo = ammoItem->GetMaxQuantity();
	auto const currentAmmo = ammoItem->GetQuantity();
	auto const ammoProvider = new AmmoProvider(0, maxAmmo, currentAmmo, ammoId);
	_ammoProviders.Add(ammoId, ammoProvider);
	return ammoProvider;
}

AActor* UWeaponManagerComponent::CreateGun(const GunItem* gunItem)
{
	auto const gunItemId = gunItem->GetId();
	auto const gunBlueprintGeneratedClass = _gunFactory->GetOrLoad(gunItemId);
	
	if(!gunBlueprintGeneratedClass)
	{
		return nullptr;
	}
	
	auto const weaponActor = AssetUtils::SpawnBlueprint<AActor>(GetWorld(), GetOwner(), gunBlueprintGeneratedClass);
	if(!weaponActor)
	{
		DevLogger::GetLoggingChannel()->LogValue("Weapon blueprint is not spawned. Gun item id:", gunItem->GetId());
		return nullptr;
	}
	
	auto const gun = dynamic_cast<IGun*>(weaponActor);

	if (!gun)
	{
		DevLogger::GetLoggingChannel()->LogValue("IGun is null. Gun item id:", gunItem->GetId());
		weaponActor->Destroy();
		return nullptr;
	}

	const WeaponId weaponId(gunItemId);
	auto const weaponType = gunItem->GetGunType();
	gun->InitializeWeapon(weaponId, weaponType, _playerId);
	gun->OutOfAmmo.AddUObject(this, &UWeaponManagerComponent::OnOutOfAmmo);

	auto const ammoId = gunItem->GetAmmoId();
	auto ammoProvider = GetAmmoProvider(ammoId);

	if (!ammoProvider)
	{
		ammoProvider = CreateAmmoProvider(gunItem);
	}

	if(!_aimProvider)
	{
		DevLogger::GetLoggingChannel()->Log("No aim provider", LogSeverity::Error);
	}

	gun->InitializeGun(ammoProvider, _aimProvider);

	AttachWeapon(weaponActor);
	
	gun->ShotPerformed.AddUObject(this, &UWeaponManagerComponent::OnGunShotPerformed);
	
	return weaponActor;
}

void UWeaponManagerComponent::AttachWeapon(AActor* weaponActor) const
{
	if(!CanAttachWeapon())
	{
		return;
	}
	
	if(_ownerSkeletalMeshComponent)
	{
		weaponActor->AttachToComponent(_ownerSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, _weaponSocketName);
	}
	else
	{
		weaponActor->AttachToComponent(_weaponPivot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

bool UWeaponManagerComponent::CanAttachWeapon() const
{
	return _ownerSkeletalMeshComponent || _weaponPivot;
}

void UWeaponManagerComponent::OnPartialInitialization()
{
	if(_isInitialized)
	{
		return;
	}
	
	if(!CanAttachWeapon() || !_aimProvider || !_playerId.IsValid())
	{
		return;
	}

	_isInitialized = true;
	
	CreateStartWeapon();
}

void UWeaponManagerComponent::CreateStartWeapon()
{
	auto const startingWeaponItem = _gunItemFactory->Get(_startWeaponId);
	TryProcessItemAsGun(startingWeaponItem);
}

void UWeaponManagerComponent::OnGunShotPerformed(const IGun* gun, const bool isAnyBulletDamaged)
{
	BpOnGunShotPerformed(isAnyBulletDamaged);

	ShotPerformed.Broadcast(gun->GetPlayerId(), gun->GetWeaponType(), isAnyBulletDamaged);
}

void UWeaponManagerComponent::OnOutOfAmmo()
{
	BpOnOutOfAmmo();

	//Try equip other weapon
	for (auto i = 0; i < _weapons.Num(); i++)
	{
		auto const weapon = _weapons[i];
		auto const gun = dynamic_cast<const IGun*>(weapon);

		if (gun)
		{
			if (!gun->IsEnoughAmmoForShot())
			{
				continue;
			}
		}

		EquipWeapon(i);
		return;
	}

	//Could not equip other weapon
	_isFireStarted = false;
}
