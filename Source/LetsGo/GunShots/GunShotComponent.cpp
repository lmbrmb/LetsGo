#include "GunShotComponent.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Utils/AssertUtils.h"

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

void UGunShotComponent::SetAimProvider(IAimProvider* aimProvider)
{
	AimProvider = aimProvider;
}

bool UGunShotComponent::GetIsLocalPlayer() const
{
	return _isLocalPlayer;
}

void UGunShotComponent::BeginPlay()
{
	Super::BeginPlay();

	GunActor = GetOwner();
	AssertIsNotNull(GunActor);

	GunOwner = GunActor->GetOwner();
	AssertIsNotNull(GunOwner);
}
