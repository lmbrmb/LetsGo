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
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);
	
	_playerSettingsManager = matchGameMode->GetPlayerSettingsManager();
	AssertIsNotNull(_playerSettingsManager);

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
	auto const weaponPivotIndex = _playerSettingsManager->GetSetting<int>(_weaponPivotIndexSettingName);
	_weaponManagerComponent->SetWeaponPivot(weaponPivotIndex);
}

void UWeaponManagerToSettingsMapping::OnWeaponPivotChanged(const int weaponPivotIndex) const
{
	_playerSettingsManager->SetSetting(_weaponPivotIndexSettingName, weaponPivotIndex);
}
