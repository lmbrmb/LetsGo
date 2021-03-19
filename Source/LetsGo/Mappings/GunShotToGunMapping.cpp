#include "GunShotToGunMapping.h"

#include "LetsGo/GunShots/GunShotComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/IGun.h"

void UGunShotToGunMapping::Map()
{
	auto const owner = GetOwner();

	auto gun = dynamic_cast<IGun*>(owner);
	AssertIsNotNull(gun);

	auto const gunShotComponent = owner->FindComponentByClass<UGunShotComponent>();
	AssertIsNotNull(gunShotComponent);

	GunShotComponent = gunShotComponent;
	GunActor = owner;
	Gun = gun;
	
	if(gun->IsWeaponInitialized())
	{
		OnWeaponInitialized(gun);
	}
	else
	{
		gun->WeaponInitialized.AddUObject(this, &UGunShotToGunMapping::OnWeaponInitialized);
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
	GunShotComponent->SetWeaponId(weapon->GetWeaponId());
	GunShotComponent->SetPlayerId(weapon->GetPlayerId());
	weapon->WeaponInitialized.RemoveAll(this);
	_isWeaponDataSet = true;
	OnPartialInitialization();
}

void UGunShotToGunMapping::OnGunInitialized(IGun* gun)
{
	GunShotComponent->SetAimProvider(gun->GetAimProvider());
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
	Bind();
	
	StartDestroyTask();
}

void UGunShotToGunMapping::Bind() const
{
	//Do nothing
}
