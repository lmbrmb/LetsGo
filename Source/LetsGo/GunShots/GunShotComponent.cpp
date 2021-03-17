#include "GunShotComponent.h"

UGunShotComponent::UGunShotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGunShotComponent::SetWeaponId(const FName& weaponId)
{
	WeaponId = weaponId;
}

void UGunShotComponent::SetPlayerId(const PlayerId& instigatorId)
{
	InstigatorId = instigatorId;
}

void UGunShotComponent::SetAimProvider(USceneComponent* aimProvider)
{
	AimProvider = aimProvider;
}
