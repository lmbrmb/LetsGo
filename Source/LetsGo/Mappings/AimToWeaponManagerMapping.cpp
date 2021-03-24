#include "AimToWeaponManagerMapping.h"

#include "LetsGo/AimProviders/AimProviderComponent.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

void UAimToWeaponManagerMapping::Map()
{
	auto const owner = GetOwner();

	_weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(_weaponManagerComponent);

	_aimProviderComponent = owner->FindComponentByClass<UAimProviderComponent>();
	AssertIsNotNull(_aimProviderComponent);

	_aimProviderComponent->Changed.AddUObject(this, &UAimToWeaponManagerMapping::OnAimProviderChanged);
	OnAimProviderChanged();
}

void UAimToWeaponManagerMapping::OnAimProviderChanged() const
{
	auto const aimProvider = _aimProviderComponent->GetAimProvider();
	_weaponManagerComponent->SetAimProvider(aimProvider);
}
