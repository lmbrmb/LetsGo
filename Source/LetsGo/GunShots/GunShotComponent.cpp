#include "GunShotComponent.h"

UGunShotComponent::UGunShotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGunShotComponent::SetWeaponId(const WeaponId& weaponId)
{
	InstigatorWeaponId = weaponId;
}

void UGunShotComponent::SetPlayerId(const PlayerId& instigatorId)
{
	InstigatorPlayerId = instigatorId;
}

void UGunShotComponent::SetAimProvider(USceneComponent* aimProvider)
{
	AimProvider = aimProvider;
}
