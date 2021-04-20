#include "WeaponManagerComponent.h"

#include "IWeapon.h"
#include "LetsGo/GameModes/ProjectGameModeBase.h"
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
	auto const projectGameModeBase = Cast<AProjectGameModeBase>(authGameMode);
	AssertIsNotNull(projectGameModeBase);
	auto const diContainer = projectGameModeBase->GetDiContainer();
	
	auto const gunFactory = diContainer->GetInstance<GunFactory>();
	_gunFactory = &gunFactory.Get();

	auto const ammoItemFactory = diContainer->GetInstance<AmmoItemFactory>();
	_ammoItemFactory = &ammoItemFactory.Get();

	auto const gunItemFactory = diContainer->GetInstance<GunItemFactory>();
	_gunItemFactory = &gunItemFactory.Get();

	auto const ammoProviderFactory = diContainer->GetInstance<AmmoProviderFactory>();
	_ammoProviderFactory = &ammoProviderFactory.Get();

	AssertIsGreaterOrEqual(_weaponEquipDuration, 0.0f);
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
	EquipWeaponImmediate(NO_WEAPON_INDEX);
}

void UWeaponManagerComponent::NextWeapon()
{
	TryEquipNextUsableWeapon(1);
}

void UWeaponManagerComponent::PreviousWeapon()
{
	TryEquipNextUsableWeapon(-1);
}

bool UWeaponManagerComponent::ChangeWeapon(const int indexModifier)
{
	return TryEquipNextUsableWeapon(indexModifier);
}

bool UWeaponManagerComponent::TryEquipNextUsableWeapon(const int indexModifier)
{
	auto const nextUsableWeaponIndex = GetNextUsableWeaponIndex(indexModifier);

	if(nextUsableWeaponIndex == UNDEFINED_INDEX)
	{
		return false;
	}
	
	RequestEquipWeapon(nextUsableWeaponIndex);
	return true;
}

bool UWeaponManagerComponent::IsChangingWeapon() const
{
	return _equipWeaponRequestReadyHandle.IsValid() || _equipWeaponTimerHandle.IsValid();
}

int UWeaponManagerComponent::GetNextUsableWeaponIndex(const int indexModifier) const
{
	if (indexModifier == 0)
	{
		DevLogger::GetLoggingChannel()->Log(__FUNCTION__ + FString("index modifier is 0"), LogSeverity::Warning);
		return UNDEFINED_INDEX;
	}

	auto const weaponsCount = _weaponActors.Num();

	if (weaponsCount < 1)
	{
		return UNDEFINED_INDEX;
	}

	auto canChangeWeapon = false;
	auto nextWeaponIndex = _weaponIndex + indexModifier;
	auto const checksCount = weaponsCount;

	for (auto i = 0; i < checksCount; i++)
	{
		if (nextWeaponIndex >= weaponsCount)
		{
			nextWeaponIndex = 0;
		}
		else if (nextWeaponIndex < 0)
		{
			nextWeaponIndex = weaponsCount - 1;
		}

		auto const gun = _guns[nextWeaponIndex];

		if (!gun || gun->IsEnoughAmmoForShot())
		{
			canChangeWeapon = true;
			break;
		}

		nextWeaponIndex += indexModifier;
	}

	if (nextWeaponIndex == _weaponIndex)
	{
		return UNDEFINED_INDEX;
	}

	return canChangeWeapon ? nextWeaponIndex : UNDEFINED_INDEX;
}

void UWeaponManagerComponent::RequestEquipWeapon(const int weaponIndex)
{
	if(IsChangingWeapon())
	{
		return;
	}
	
	_nextWeaponIndex = weaponIndex;

	if(_weaponActor)
	{
		ActorUtils::SetEnabled(_weaponActor, false);
	}
	
	if (_weapon)
	{
		if (!_weapon->IsRequestReady())
		{
			_equipWeaponRequestReadyHandle = _weapon->RequestReady.AddUObject(this, &UWeaponManagerComponent::EquipWeaponOnRequestReady);
			return;
		}
	}

	EquipWeaponOnRequestReady();
}

void UWeaponManagerComponent::EquipWeaponOnRequestReady()
{
	if (_equipWeaponRequestReadyHandle.IsValid())
	{
		AssertIsNotNull(_weapon);
		_weapon->RequestReady.Remove(_equipWeaponRequestReadyHandle);
		_equipWeaponRequestReadyHandle.Reset();
	}

	HolsterWeapon();

	if(_weaponEquipDuration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			_equipWeaponTimerHandle,
			this,
			&UWeaponManagerComponent::EquipWeaponOnTimer,
			_weaponEquipDuration,
			false
		);
		return;
	}

	EquipWeaponOnTimer();
}

void UWeaponManagerComponent::EquipWeaponOnTimer()
{
	_equipWeaponTimerHandle.Invalidate();
	EquipWeaponImmediate(_nextWeaponIndex);
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

int UWeaponManagerComponent::GetWeaponsCount() const
{
	return _weapons.Num();
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

	SetWeaponPivot(nextIndex);
}

void UWeaponManagerComponent::SetWeaponPivot(const int index)
{
	if (_weaponPivotIndex == index)
	{
		return;
	}
	
	AssertIndexIsInArrayBounds(index, _weaponPivots);

	_weaponPivotIndex = index;
	_weaponPivot = _weaponPivots[_weaponPivotIndex];

	for (auto weaponActor : _weaponActors)
	{
		AttachWeapon(weaponActor);
	}
}

IAimProvider* UWeaponManagerComponent::GetAimProvider() const
{
	return _aimProvider;
}

void UWeaponManagerComponent::EquipWeaponImmediate(const int weaponIndex)
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
	_gun = noWeapon ? nullptr : _guns[weaponIndex];

	if(_weaponActor)
	{
		ActorUtils::SetEnabled(_weaponActor, true);
		
		if (_isFireStarted)
		{
			StartWeaponFire();
		}
		WeaponEquipped.Broadcast();
		BpOnWeaponEquipped();
		return;
	}

	WeaponHolstered.Broadcast();
	BpOnWeaponHolstered();
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
	auto haveThisWeapon = false;
	auto haveGunWithSameAmmo = false;

	auto const gunsCount = _guns.Num();
	for (auto i = 0; i < gunsCount; i++)
	{
		auto const weapon = _weapons[i];
		
		if (weapon->GetWeaponId().GetId() == gunId)
		{
			haveThisWeapon = true;
		}
		
		auto const gun = _guns[i];
		if(!gun)
		{
			continue;
		}

		if (gun->GetAmmoProvider()->GetAmmoId() == ammoId)
		{
			haveGunWithSameAmmo = true;
		}
	}

	if (haveThisWeapon || haveGunWithSameAmmo)
	{
		auto const ammoItem = _ammoItemFactory->Get(ammoId);
		TryProcessItemAsAmmo(ammoItem);
	}

	if(haveThisWeapon)
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
	_guns.Add(gun);
	
	auto const weaponIndex = _weaponActors.Add(gunActor);

	if (noWeapons || _shouldEquipWeaponOnPickup)
	{
		RequestEquipWeapon(weaponIndex);
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

IAmmoProvider* UWeaponManagerComponent::GetAmmoProvider(const FName ammoId)
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

IAmmoProvider* UWeaponManagerComponent::CreateAmmoProvider(const GunItem* gunItem)
{
	auto const ammoId = gunItem->GetAmmoId();
	auto const ammoItem = _ammoItemFactory->Get(ammoId);
	auto const maxAmmo = ammoItem->GetMaxQuantity();
	auto const isInfiniteAmmo = maxAmmo == INT_MAX;
	auto const currentAmmo = gunItem->GetInitialAmmoCount();
	
	auto const ammoProvider =
		isInfiniteAmmo ?
		_ammoProviderFactory->CreateInfiniteAmmoProvider(ammoId) :
		_ammoProviderFactory->CreateAmmoProvider(ammoId, maxAmmo, currentAmmo);
	_ammoProviders.Add(ammoId, ammoProvider);
	return ammoProvider;
}

IAmmoProvider* UWeaponManagerComponent::CreateAmmoProvider(const AmmoItem* ammoItem)
{
	auto const ammoId = ammoItem->GetId();
	auto const maxAmmo = ammoItem->GetMaxQuantity();
	auto const currentAmmo = ammoItem->GetQuantity();
	auto const ammoProvider = _ammoProviderFactory->CreateAmmoProvider(ammoId, maxAmmo, currentAmmo);
	_ammoProviders.Add(ammoId, ammoProvider);
	return ammoProvider;
}

AActor* UWeaponManagerComponent::CreateGun(const GunItem* gunItem)
{
	auto const gunItemId = gunItem->GetId();
	auto const gunBlueprintGeneratedClass = _gunFactory->GetOrLoad(gunItemId);
	AssertIsNotNull(gunBlueprintGeneratedClass, nullptr);

	auto const gunActor = AssetUtils::SpawnBlueprint<AActor>(
		GetWorld(),
		GetOwner(),
		gunBlueprintGeneratedClass,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	if(!gunActor)
	{
		DevLogger::GetLoggingChannel()->LogValue("Weapon blueprint is not spawned. Gun item id:", gunItem->GetId());
		return nullptr;
	}
	
	auto const gun = dynamic_cast<IGun*>(gunActor);

	if (!gun)
	{
		DevLogger::GetLoggingChannel()->LogValue("IGun is null. Gun item id:", gunItem->GetId());
		gunActor->Destroy();
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

	AttachWeapon(gunActor);
	
	gun->ShotPerformed.AddUObject(this, &UWeaponManagerComponent::OnGunShotPerformed);
	
	return gunActor;
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

	auto const isWeaponChanged = TryEquipNextUsableWeapon(1);

	if(!isWeaponChanged)
	{
		_isFireStarted = false;
	}
}
