#include "GunShotComponent.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Physics/RigidbodyComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

const FName UGunShotComponent::ForceName = FName("Shot");

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

void UGunShotComponent::ApplyForce(AActor* actor, const FVector& direction) const
{
	if(!ImpactForceCurve)
	{
		return;
	}

	auto const rigidBodyComponent = actor->FindComponentByClass<URigidBodyComponent>();
	if(!rigidBodyComponent)
	{
		return;
	}

	rigidBodyComponent->AddForce(ForceName, direction, ImpactForceCurve, ImpactForceCurveMagnitudeMultiplier, ImpactForceCurveTimeMultiplier);
}
