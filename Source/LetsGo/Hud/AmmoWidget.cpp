#include "AmmoWidget.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/PlayerControllers/MatchPlayerController.h"

void UAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
	}

	_weaponManagerComponent = nullptr;
	
	if (avatar)
	{
		_weaponManagerComponent = avatar->FindComponentByClass<UWeaponManagerComponent>();

		if (_weaponManagerComponent)
		{
			_weaponManagerComponent->WeaponEquipped.AddUObject(this, &UAmmoWidget::OnWeaponEquipped);
			OnWeaponEquipped();
		}
	}
}

void UAmmoWidget::OnWeaponEquipped()
{
	if(_gun)
	{
		auto const ammoProvider = _gun->GetAmmoProvider();
		AssertIsNotNull(ammoProvider);
		ammoProvider->Changed.RemoveAll(this);
	}
	
	_gun = _weaponManagerComponent->GetCurrentGun();

	if(!_gun)
	{
		return;
	}
	
	auto const ammoProvider = _gun->GetAmmoProvider();
	AssertIsNotNull(ammoProvider);
	
	auto const ammoId = ammoProvider->GetAmmoId();
	BpOnGunChanged(ammoId);
	
	ammoProvider->Changed.AddUObject(this, &UAmmoWidget::OnAmmoCountChanged);
	auto const ammoCount = ammoProvider->GetCurrent();
	OnAmmoCountChanged(ammoCount);
}

void UAmmoWidget::OnAmmoCountChanged(const int ammoCount)
{
	BpOnAmmoCountChanged(ammoCount);
}
