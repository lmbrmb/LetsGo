#include "GunShotToGunMapping.h"

#include "LetsGo/GunShots/GunShotComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/Gun.h"
#include "LetsGo/WeaponSystem/GunV1.h"
#include "LetsGo/WeaponSystem/GunV2.h"
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
	_gun->ShotRequested.AddUObject(_gunShotComponent, &UGunShotComponent::OnShotRequested);

	BindOnShotPerformed();
	
	StartDestroyTask();
}

void UGunShotToGunMapping::BindOnShotPerformed() const
{
	// Can't bind to interface method:
	// _gunShotComponent->ShotPerformed.AddUObject(_gun, &IGun::OnShotPerformed);
	// Error: "You cannot use UObject method delegates with raw pointers"
	
	auto const gunV1 = Cast<AGunV1>(_gun);
	if (gunV1)
	{
		_gunShotComponent->ShotPerformed.AddUObject(gunV1, &AGunV1::OnShotPerformed);
		return;
	}
	
	auto const gunV2 = Cast<AGunV2>(_gun);
	if(gunV2)
	{
		_gunShotComponent->ShotPerformed.AddUObject(gunV2, &AGunV2::OnShotPerformed);
		return;
	}

	DevLogger::GetLoggingChannel()->Log("Can't bind gun to shot. Unknown gun implementation", LogSeverity::Error);
}
