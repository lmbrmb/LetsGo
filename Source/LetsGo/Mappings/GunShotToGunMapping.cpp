#include "GunShotToGunMapping.h"

#include "LetsGo/GunShots/GunShotComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/Gun.h"
#include "LetsGo/WeaponSystem/Weapon.h"

void UGunShotToGunMapping::Map()
{
	auto const owner = GetOwner();

	auto const weapon = Cast<IWeapon>(owner);
	AssertIsNotNull(weapon);

	auto const gun = Cast<IGun>(owner);
	AssertIsNotNull(gun);

	auto const gunShotComponent = owner->FindComponentByClass<UGunShotComponent>();
	AssertIsNotNull(gunShotComponent);

	_gunShotComponent = gunShotComponent;

	_gun = gun;
	
	if(weapon->IsWeaponInitialized())
	{
		OnWeaponInitialized(weapon);
	}
	else
	{
		weapon->WeaponInitialized.AddUObject(this, &UGunShotToGunMapping::OnWeaponInitialized);
	}

	if(gun->IsGunInitialized())
	{
		OnGunInitialized(gun);
	}
	else
	{
		gun->GunInitialized.AddUObject(this, &UGunShotToGunMapping::OnGunInitialized);
	}
}

void UGunShotToGunMapping::OnWeaponInitialized(IWeapon* weapon)
{
	_gunShotComponent->SetWeaponId(weapon->GetId());
	_gunShotComponent->SetPlayerId(weapon->GetPlayerId());
	weapon->WeaponInitialized.RemoveAll(this);
	_isWeaponDataSet = true;
	OnPartialInitialization();
}

void UGunShotToGunMapping::OnGunInitialized(IGun* gun)
{
	_gunShotComponent->SetAimProvider(gun->GetAimProvider());
	gun->GunInitialized.RemoveAll(this);
	_isGunDataSet = true;
	OnPartialInitialization();
}

void UGunShotToGunMapping::OnPartialInitialization()
{
	if (!_isGunDataSet || !_isWeaponDataSet)
	{
		return;
	}
	
	_gun->ShotPerformed.AddUObject(_gunShotComponent, &UGunShotComponent::OnShot);
	StartDestroyTask();
}
