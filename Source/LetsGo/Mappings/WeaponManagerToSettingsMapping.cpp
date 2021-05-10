#include "WeaponManagerToSettingsMapping.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"
#include "LetsGo/Settings/PlayerSettingsManager.h"

void UWeaponManagerToSettingsMapping::Map()
{
	auto const owner = GetOwner();
	_weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(_weaponManagerComponent);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	_weaponManagerComponent->WeaponPivotChanged.AddUObject(this, &UWeaponManagerToSettingsMapping::OnWeaponPivotChanged);

	if(_weaponManagerComponent->IsInitialized())
	{
		OnWeaponManagerInitialized();
		return;
	}

	_weaponManagerComponent->Initialized.AddUObject(this, &UWeaponManagerToSettingsMapping::OnWeaponManagerInitialized);
}

void UWeaponManagerToSettingsMapping::OnWeaponManagerInitialized() const
{
	auto const playerSettings = _matchGameMode->GetPlayerSettings();
	AssertIsNotNull(playerSettings);

	auto const weaponPivotIndex = playerSettings->CenterWeapon ? 1 : 0;
	_weaponManagerComponent->SetWeaponPivot(weaponPivotIndex);

	auto const shouldEquipWeaponOnPickup = playerSettings->ShouldEquipWeaponOnPickup;
	_weaponManagerComponent->SetShouldEquipWeaponOnPickup(shouldEquipWeaponOnPickup);
}

void UWeaponManagerToSettingsMapping::OnWeaponPivotChanged(const int weaponPivotIndex) const
{
	_matchGameMode->GetPlayerSettings()->CenterWeapon = weaponPivotIndex == 1;
}
