#include "ProjectileShotComponent.h"

#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/AssetUtils.h"

void UProjectileShotComponent::OnShotRequested(const USceneComponent* firePivot)
{
	AssertIsNotNull(firePivot);
	AssertIsNotNull(_projectileBlueprint);

	auto const projectile = AssetUtils::SpawnBlueprint<AProjectile>(
		GetWorld(),
		nullptr,
		_projectileBlueprint,
		firePivot->GetComponentTransform(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	projectile->Hit.AddUObject(this, &UProjectileShotComponent::OnProjectileHit);
}

void UProjectileShotComponent::OnProjectileHit(AProjectile* projectile, const FHitResult& hitResult) const
{
	auto const isAnyBulletDamaged = false;

	// TODO: area damage
	
	if(hitResult.Actor.IsValid())
	{
		auto const hitActor = hitResult.Actor.Get();
		auto const healthComponent = hitActor->FindComponentByClass<UHealthComponent>();

		if(healthComponent)
		{
			const Damage damage(InstigatorPlayerId, InstigatorWeaponType, 75);
			healthComponent->TryInjure(damage);
		}
	}

	ShotPerformed.Broadcast(isAnyBulletDamaged);

	projectile->Destroy();
}
