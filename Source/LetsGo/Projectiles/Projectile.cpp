#include "Projectile.h"

#include "LetsGo/Utils/AssertUtils.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectile::OnCollision(const FHitResult& hitResult)
{
	Hit.Broadcast(this, hitResult);
	BpHit(hitResult);
	Destroy();
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SubscribeLifeTimeExpire();
}

void AProjectile::BeginDestroy()
{
	Super::BeginDestroy();

	UnsubscribeLifeTimeExpire();
}

void AProjectile::LifeTimeExpireOnTimer()
{
	LifeTimeExpired.Broadcast(this);
	Destroy();
}

void AProjectile::SubscribeLifeTimeExpire()
{
	AssertIsFalse(_lifeTimeTimerHandle.IsValid());
	auto const world = GetWorld();
	AssertIsNotNull(world);
	AssertIsGreater(_lifeTimeSeconds, 0.0f);
	world->GetTimerManager().SetTimer(_lifeTimeTimerHandle, this, &AProjectile::LifeTimeExpireOnTimer, _lifeTimeSeconds);
}

void AProjectile::UnsubscribeLifeTimeExpire()
{
	if (!_lifeTimeTimerHandle.IsValid())
	{
		return;
	}
	
	auto const world = GetWorld();
	if (!world)
	{
		return;
	}

	world->GetTimerManager().ClearTimer(_lifeTimeTimerHandle);
}
