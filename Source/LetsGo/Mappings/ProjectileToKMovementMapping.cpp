#include "ProjectileToKMovementMapping.h"

#include "LetsGo/GunShots/ProjectileShotComponent.h"
#include "LetsGo/Movement/KinematicMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UProjectileToKMovementMapping::Map()
{
	auto const owner = GetOwner();

	auto const projectile = Cast<AProjectile>(owner);
	AssertIsNotNull(projectile);

	auto const kinematicMovementComponent = owner->FindComponentByClass<UKinematicMovementComponent>();
	AssertIsNotNull(kinematicMovementComponent);

	kinematicMovementComponent->Collision.AddUObject(projectile, &AProjectile::OnCollision);
}
