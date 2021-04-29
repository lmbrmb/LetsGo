#include "GunShotComponent.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Utils/AssertUtils.h"

UGunShotComponent::UGunShotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGunShotComponent::SetWeaponType(const WeaponType& weaponType)
{
	InstigatorWeaponType = weaponType;
}

void UGunShotComponent::SetPlayerId(const PlayerId& instigatorId)
{
	InstigatorPlayerId = instigatorId;
}

void UGunShotComponent::SetAimProvider(IAimProvider* aimProvider)
{
	AimProvider = aimProvider;
}

void UGunShotComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const componentOwner = GetOwner();
	GunActor = componentOwner;
	AssertIsNotNull(GunActor);

	GunOwner = GunActor->GetOwner();
	AssertIsNotNull(GunOwner);
}
