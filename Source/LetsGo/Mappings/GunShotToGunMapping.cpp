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
		OnWeaponInitialized();
	}
	else
	{
		gun->WeaponInitialized.AddUObject(this, &UGunShotToGunMapping::OnWeaponInitialized);
	}

	if(gun->IsGunInitialized())
	{
		OnGunInitialized();
	}
	else
	{
		gun->GunInitialized.AddUObject(this, &UGunShotToGunMapping::OnGunInitialized);
	}
}

void UGunShotToGunMapping::OnWeaponInitialized()
{
	GunShotComponent->SetWeaponType(Gun->GetWeaponType());
	GunShotComponent->SetPlayerId(Gun->GetPlayerId());
	Gun->WeaponInitialized.RemoveAll(this);
	_isWeaponDataSet = true;
	OnPartialInitialization();
}

void UGunShotToGunMapping::OnGunInitialized()
{
	auto const aimProvider = Gun->GetAimProvider();
	GunShotComponent->SetAimProvider(aimProvider);
	Gun->GunInitialized.RemoveAll(this);
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
	Gun->ShotRequested.AddUObject(GunShotComponent, &UGunShotComponent::OnShotRequested);
	GunShotComponent->ShotPerformed.AddRaw(Gun, &IGun::OnShotPerformed);
}
