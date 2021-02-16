#include "GunShotComponent.h"
#include "LetsGo/WeaponSystem/Gun.h"

UGunShotComponent::UGunShotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGunShotComponent::BeginPlay()
{
	Super::BeginPlay();
	auto const owner = GetOwner();
	auto const gun = dynamic_cast<AGun*>(owner);
	gun->ShotPerformed.AddUObject(this, &UGunShotComponent::OnShot);
	Init();
}

// Template methods down below

void UGunShotComponent::Init()
{
	//Do nothing
}

void UGunShotComponent::OnShot(FTransform pivotTransform, FVector shotDirection)
{
	//Do nothing
}
