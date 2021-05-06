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

bool UWeaponManagerComponent::IsInitialized() const
{
	return _isInitialized;
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
	EquipNextUsableWeapon(1);
}

void UWeaponManagerComponent::PreviousWeapon()
{
	EquipNextUsableWeapon(-1);
}

void UWeaponManagerComponent::ChangeWeapon(const int indexModifier)
{
	EquipNextUsableWeapon(indexModifier);
}

void UWeaponManagerComponent::ChangeWeapon(const FName& weaponId)
{
	auto const weaponIndex = GetWeaponIndex(weaponId);
	RequestEquipWeapon(weaponIndex);
}

void UWeaponManagerComponent::EquipNextUsableWeapon(const int indexModifier)
{
	auto const nextUsableWeaponIndex = GetNextUsableWeaponIndex(indexModifier);
	RequestEquipWeapon(nextUsableWeaponIndex);
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

	return canChangeWeapon ? nextWeaponIndex : UNDEFINED_INDEX;
}

void UWeaponManagerComponent::RequestEquipWeapon(const int weaponIndex)
{
	if (weaponIndex == UNDEFINED_INDEX || _weaponIndex == weaponIndex)
	{
		return;
	}

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
	auto const weaponsCount = _weapons.Num();
	AssertIsGreater(weaponsCount, 0);

	if (_equipWeaponRequestReadyHandle.IsValid())
	{
		AssertIsNotNull(_weapon);
		_weapon->RequestReady.Remove(_equipWeaponRequestReadyHandle);
		_equipWeaponRequestReadyHandle.Reset();
	}

	HolsterWeapon();

	auto const shouldWait = _weaponEquipDuration > 0 && weaponsCount > 1;
	if(shouldWait)
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

void UWeaponManagerComponent::InitializeWeaponPivots(const TArray<USceneComponent*>& weaponPivots)
{
	AssertContainerIsNotEmpty(weaponPivots);
	
	for (auto weaponPivot : weaponPivots)
	{
		_weaponPivots.Add(weaponPivot);
	}

	SetWeaponPivot(0);
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

FName UWeaponManagerComponent::GetCurrentWeaponId() const
{
	if(_weapon)
	{
		return _weapon->GetWeaponId().GetId();
	}

	return FName();
}

TArray<IWeapon*>& UWeaponManagerComponent::GetWeapons()
{
	return _weapons;
}

TArray<IGun*>& UWeaponManagerComponent::GetGuns()
{
	return _guns;
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

	WeaponPivotChanged.Broadcast(_weaponPivotIndex);
}

void UWeaponManagerComponent::SetWeaponPivot(const int weaponPivotIndex)
{
	if (_weaponPivotIndex == weaponPivotIndex)
	{
		return;
	}
	
	AssertIndexIsInArrayBounds(weaponPivotIndex, _weaponPivots);

	_weaponPivotIndex = weaponPivotIndex;
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

void UWeaponManagerComponent::DisableWeapon()
{
	StopFire();

	if(_equipWeaponTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_equipWeaponTimerHandle);
		_equipWeaponTimerHandle.Invalidate();
	}

	if(_equipWeaponRequestReadyHandle.IsValid())
	{
		AssertIsNotNull(_weapon);
		_weapon->RequestReady.Remove(_equipWeaponRequestReadyHandle);
		_equipWeaponRequestReadyHandle.Reset();
	}

	HolsterWeapon();
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

/// <summary>
/// Creates gun if not exist in weapons collection.
/// Adds ammo if already have this gun.
/// Can carry only one weapon of type.
///	Equip gun if should.
/// </summary>
bool UWeaponManagerComponent::TryProcessItemAsGun(Item* item)
{
	auto const gunItem = dynamic_cast<GunItem*>(item);
	if (gunItem == nullptr)
	{
		return false;
	}

	auto const gunId = gunItem->GetId();
	auto const ammoId = gunItem->GetAmmoId();
	auto haveGunWithSameAmmo = false;
	int weaponIndex = UNDEFINED_INDEX;
	auto const weaponsCount = _weapons.Num();
	auto const noWeapons = weaponsCount <= 0;

	for (auto i = 0; i < weaponsCount; i++)
	{
		auto const weapon = _weapons[i];
		
		if (weapon->GetWeaponId().GetId() == gunId)
		{
			weaponIndex = i;
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

	if (haveGunWithSameAmmo)
	{
		auto const ammoItem = _ammoItemFactory->Get(ammoId);
		TryProcessItemAsAmmo(ammoItem);
	}

	auto const gunIsNotFound = weaponIndex == UNDEFINED_INDEX;
	if (gunIsNotFound)
	{
		auto const gunActor = CreateGun(gunItem);

		if (!gunActor)
		{
			return false;
		}

		ActorUtils::SetEnabled(gunActor, false);

		// No null check, CreateGun already checked that created gun is IGun
		auto const gun = dynamic_cast<IGun*>(gunActor);
		weaponIndex = _weapons.Add(gun);
		_guns.Add(gun);
		_weaponActors.Add(gunActor);
	}

	auto const currentWeaponIsDifferent = GetCurrentWeaponId() != gunId;
	auto const shouldEquipWeapon = noWeapons || (_shouldEquipWeaponOnPickup && currentWeaponIsDifferent);

	if (shouldEquipWeapon)
	{
		AssertIsNotEqual(weaponIndex, UNDEFINED_INDEX, true);
		RequestEquipWeapon(weaponIndex);
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
	gun->ShotRequested.AddUObject(this, &UWeaponManagerComponent::OnGunShotRequested);

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

	Initialized.Broadcast();

	CreateStartWeapon();
}

void UWeaponManagerComponent::CreateStartWeapon()
{
	auto const startWeaponItem = _gunItemFactory->Get(_startWeaponId);
	TryProcessItemAsGun(startWeaponItem);
}

void UWeaponManagerComponent::OnGunShotPerformed(const IGun* gun, const bool isAnyBulletDamaged)
{
	BpOnGunShotPerformed(isAnyBulletDamaged);
	ShotPerformed.Broadcast(gun->GetPlayerId(), gun->GetWeaponType(), isAnyBulletDamaged);
}

void UWeaponManagerComponent::OnGunShotRequested(const USceneComponent* firePivot) const
{
	ShotRequested.Broadcast();
}

void UWeaponManagerComponent::OnOutOfAmmo()
{
	BpOnOutOfAmmo();

	auto const weaponIndex = _weaponIndex;
	EquipNextUsableWeapon(1);

	if(weaponIndex == _weaponIndex && !IsChangingWeapon())
	{
		_isFireStarted = false;
	}
}

int UWeaponManagerComponent::GetWeaponIndex(const FName& weaponId)
{
	auto const weaponsCount = _weapons.Num();
	for (auto i = 0; i < weaponsCount; i++)
	{
		auto const weapon = _weapons[i];

		if (weapon->GetWeaponId().GetId() == weaponId)
		{
			return i;
		}
	}

	return UNDEFINED_INDEX;
}
