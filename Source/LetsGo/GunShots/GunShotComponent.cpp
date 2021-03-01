#include "GunShotComponent.h"
#include "LetsGo/WeaponSystem/Gun.h"

UGunShotComponent::UGunShotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGunShotComponent::BeginPlay()
{
	Super::BeginPlay();
	auto const owner = GetOwner();
	auto const gun = Cast<AGun>(owner);
	gun->ShotPerformed.AddUObject(this, &UGunShotComponent::OnShot);
	Init();
}
