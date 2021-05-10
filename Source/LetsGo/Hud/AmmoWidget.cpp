#include "AmmoWidget.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/PlayerControllers/MatchPlayerController.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAmmoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AssertIsNotNull(localPlayerController);

	auto const matchPlayerController = Cast<AMatchPlayerController>(localPlayerController);
	AssertIsNotNull(matchPlayerController);

	matchPlayerController->AvatarChanged.AddUObject(this, &UAmmoWidget::OnAvatarChanged);
	auto const avatar = matchPlayerController->GetAvatar();
	OnAvatarChanged(avatar);
}

void UAmmoWidget::OnAvatarChanged(const AAvatar* avatar)
{
	if (_weaponManagerComponent)
	{
		_weaponManagerComponent->WeaponEquipped.RemoveAll(this);
		_weaponManagerComponent->WeaponHolstered.RemoveAll(this);
	}

	_weaponManagerComponent = nullptr;
	
	if (avatar)
	{
		_weaponManagerComponent = avatar->FindComponentByClass<UWeaponManagerComponent>();

		if (_weaponManagerComponent)
		{
			_weaponManagerComponent->WeaponEquipped.AddUObject(this, &UAmmoWidget::OnWeaponEquipped);
			_weaponManagerComponent->WeaponHolstered.AddUObject(this, &UAmmoWidget::OnWeaponHolstered);
			OnWeaponEquipped();
		}
	}
}

void UAmmoWidget::OnWeaponEquipped()
{
	BpOnWeaponEquipped();

	if(_gun)
	{
		auto const ammoProvider = _gun->GetAmmoProvider();
		AssertIsNotNull(ammoProvider);
		ammoProvider->AmmoCountChanged.RemoveAll(this);
	}

	if(_weaponManagerComponent)
	{
		_gun = _weaponManagerComponent->GetCurrentGun();
	}

	if(!_gun)
	{
		return;
	}
	
	auto const ammoProvider = _gun->GetAmmoProvider();
	AssertIsNotNull(ammoProvider);
	
	auto const ammoId = ammoProvider->GetAmmoId();
	BpOnGunEquipped(ammoId);

	ammoProvider->AmmoCountChanged.AddUObject(this, &UAmmoWidget::OnAmmoCountChanged);
	auto const ammoCount = ammoProvider->GetCurrent();
	OnAmmoCountChanged(ammoCount);
}

void UAmmoWidget::OnWeaponHolstered()
{
	BpOnWeaponHolstered();
}

void UAmmoWidget::OnAmmoCountChanged(const int ammoCount)
{
	BpOnAmmoCountChanged(ammoCount);
}
