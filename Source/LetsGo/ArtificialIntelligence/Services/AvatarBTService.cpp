#include "AvatarBTService.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/MathUtils.h"

void UAvatarBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if(!_isCollisionShapeInitialized)
	{
		_collisionShape.SetSphere(_detectionRadius);
		_isCollisionShapeInitialized = true;
	}

	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent);

	Update(blackboardComponent);
}

void UAvatarBTService::Update(UBlackboardComponent* blackboardComponent)
{
	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);

	auto const selfAvatar = Cast<AAvatar>(selfActorObject);
	AssertIsNotNull(selfAvatar);

	_enemies.Empty();
	_pickupItems.Empty();

	auto const selfLocation = selfAvatar->GetActorLocation();

	Detect(selfAvatar, selfLocation);
	UpdatePickup(selfAvatar, blackboardComponent, selfLocation);
	UpdateEnemy(blackboardComponent, selfAvatar, selfLocation);
}

bool UAvatarBTService::IsNeedHealth(UHealthComponent* healthComponent) const
{
	auto const currentHealth = healthComponent->GetCurrentValue();
	auto const isLowHealth = currentHealth < _lowHealthValue;
	return isLowHealth;
}

bool UAvatarBTService::IsNeedWeapon(UWeaponManagerComponent* weaponManagerComponent) const
{
	if (weaponManagerComponent->IsChangingWeapon())
	{
		return false;
	}

	auto const currentWeapon = weaponManagerComponent->GetCurrentWeapon();

	if (!currentWeapon)
	{
		return true;
	}

	auto const isLowTierWeapon = IsLowTierWeapon(currentWeapon);
	if(isLowTierWeapon)
	{
		return true;
	}

	auto const currentGun = weaponManagerComponent->GetCurrentGun();
	if (currentGun)
	{
		auto const isEnoughAmmo = IsEnoughAmmo(currentGun);
		return !isEnoughAmmo;
	}

	return true;
}

bool UAvatarBTService::IsLowTierWeapon(IWeapon* weapon) const
{
	return _lowTierWeapons.Contains(weapon->GetWeaponId().GetId());
}

bool UAvatarBTService::IsEnoughAmmo(IGun* gun) const
{
	auto const ammoProvider = gun->GetAmmoProvider();

	auto const maxAmmo = ammoProvider->GetMax();
	AssertIsGreater(maxAmmo, 0, false);

	auto const ammoPercent = ammoProvider->GetCurrent() / maxAmmo;
	return ammoPercent >= _minAmmoPercent;
}

void UAvatarBTService::Detect(
	AAvatar* selfAvatar,
	const FVector& selfLocation
)
{
	_hitResults.Empty();
	auto const hasBlockingHits = GetWorld()->SweepMultiByChannel(
		_hitResults,
		selfLocation,
		selfLocation,
		FQuat::Identity,
		_traceCollisionChannel,
		_collisionShape
	);

	if (!hasBlockingHits)
	{
		return;
	}

	auto const selfTeamId = selfAvatar->GetTeamId();

	for (auto const& hitResult : _hitResults)
	{
		if (!hitResult.Actor.IsValid())
		{
			continue;
		}

		auto const hitActor = hitResult.GetActor();
		if (hitActor == selfAvatar)
		{
			continue;
		}

		if(TryProcessPickup(hitActor, selfLocation))
		{
			continue;
		}

		TryProcessEnemy(selfAvatar, hitActor, selfTeamId);
	}
}

bool UAvatarBTService::TryProcessEnemy(
	AActor* selfActor,
	AActor* otherActor,
	const TeamId& selfTeamId
)
{
	auto const otherAvatar = Cast<AAvatar>(otherActor);

	if (!otherAvatar)
	{
		return false;
	}

	TryAddEnemy(selfActor, otherAvatar, selfTeamId);

	return true;
}

void UAvatarBTService::TryAddEnemy(
	AActor* selfActor,
	AAvatar* enemyAvatar,
	const TeamId& selfTeamId
)
{
	if (enemyAvatar->GetTeamId() == selfTeamId)
	{
		return;
	}

	auto const isAlive = IsAlive(enemyAvatar);
	if (!isAlive)
	{
		return;
	}

	auto const isEnemyInLineOfSight = IsInLineOfSight(selfActor, enemyAvatar);
	if(!isEnemyInLineOfSight)
	{
		return;
	}

	_enemies.Add(enemyAvatar);
}

bool UAvatarBTService::TryProcessPickup(
	AActor* otherActor,
	const FVector& selfLocation
)
{
	auto const pickupItem = Cast<APickupItem>(otherActor);

	if (!pickupItem)
	{
		return false;
	}

	TryAddPickup(pickupItem);

	return true;
}

void UAvatarBTService::TryAddPickup(
	APickupItem* pickupItem
)
{
	auto const isPickupValid = IsPickupValid(pickupItem);
	if(!isPickupValid)
	{
		return;
	}

	auto const isKnownPickup = IsKnownPickup(pickupItem);
	if (!isKnownPickup)
	{
		return;
	}

	_pickupItems.Add(pickupItem);
}

bool UAvatarBTService::IsKnownPickup(APickupItem* pickupItem) const
{
	auto const pickupId = pickupItem->GetId();

	if (_weaponPickups.Contains(pickupId))
	{
		return true;
	}

	if (_normalHealthPickups.Contains(pickupId))
	{
		return true;
	}
	
	if (_superHealthPickups.Contains(pickupId))
	{
		return true;
	}

	return false;
}

bool UAvatarBTService::IsPickupValid(
	APickupItem* pickupItem
)
{
	return pickupItem && !pickupItem->IsTaken();
}

bool UAvatarBTService::CanTakePickup(
	APickupItem* pickupItem,
	const bool isNeedHealth,
	const bool isNeedWeapon
) const
{
	auto const pickupId = pickupItem->GetId();
	if (_weaponPickups.Contains(pickupId))
	{
		return isNeedWeapon;
	}

	if (_normalHealthPickups.Contains(pickupId))
	{
		return isNeedHealth;
	}

	if (_superHealthPickups.Contains(pickupId))
	{
		return true;
	}

	return true;
}

void UAvatarBTService::UpdateEnemy(
	UBlackboardComponent* blackboardComponent,
	AActor* selfActor,
	const FVector& selfLocation
)
{
	//Assumption: enemies array contains only enemies in line of sight

	auto const enemiesCount = _enemies.Num();
	if (enemiesCount <= 0)
	{
		blackboardComponent->SetValueAsObject(_enemyActorKeyName, nullptr);
		blackboardComponent->SetValueAsBool(_isEnemyInLineOfSightKeyName, false);
		return;
	}

	auto const previousEnemy = blackboardComponent->GetValueAsObject(_enemyActorKeyName);
	AActor* targetEnemy = nullptr;

	if(enemiesCount == 1)
	{
		targetEnemy = _enemies[0];
	}
	else
	{
		auto const selfForward = selfActor->GetActorForwardVector();

		float maxEnemyFactor = INT_MIN;

		for (auto const enemy : _enemies)
		{
			auto const enemyFactor = GetEnemyFactor(enemy, selfLocation, selfForward);

			if (maxEnemyFactor < enemyFactor)
			{
				maxEnemyFactor = enemyFactor;
				targetEnemy = enemy;
			}
		}
	}

	if (previousEnemy && previousEnemy == targetEnemy)
	{
		return;
	}

	blackboardComponent->SetValueAsObject(_enemyActorKeyName, targetEnemy);
	blackboardComponent->SetValueAsFloat(_enemyDetectionTimeKeyName, GetWorld()->GetTimeSeconds());
	blackboardComponent->SetValueAsBool(_isEnemyInLineOfSightKeyName, true);
}

float UAvatarBTService::GetEnemyFactor(
	AActor* enemyActor,
	const FVector& selfLocation,
	const FVector& selfForward
) const
{
	auto const enemyDirection = (enemyActor->GetActorLocation() - selfLocation).GetSafeNormal();
	auto const enemyDot = FVector::DotProduct(selfForward, enemyDirection);
	auto const dotFactor = MathUtils::GetNormalizedValue(enemyDot, -1, 1);
	auto const squaredDistance = (enemyActor->GetActorLocation() - selfLocation).SizeSquared();
	auto const distanceFactor =
		FMath::IsNearlyZero(squaredDistance)
		? 0
		: FMath::Clamp(_distanceSquaredBaseFactor / squaredDistance, 0.0f, 1.0f);
	auto const factor = dotFactor + distanceFactor;
	return factor;
}

void UAvatarBTService::UpdatePickup(
	AActor* selfActor,
	UBlackboardComponent* blackboardComponent,
	const FVector& selfLocation
) const
{
	auto const healthComponent = selfActor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	auto const weaponManagerComponent = selfActor->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManagerComponent);

	auto const isNeedHealth = IsNeedHealth(healthComponent);
	auto const isNeedWeapon = IsNeedWeapon(weaponManagerComponent);

	auto const currentPickupActor = blackboardComponent->GetValueAsObject(_pickupActorKeyName);
	if (currentPickupActor)
	{
		auto const pickupItem = Cast<APickupItem>(currentPickupActor);
		AssertIsNotNull(pickupItem);
		auto const isPickupValid = IsPickupValid(pickupItem);
		if (isPickupValid)
		{
			auto const canTakePickup = CanTakePickup(pickupItem, isNeedHealth, isNeedWeapon);
			if(canTakePickup)
			{
				return;
			}
		}
	}

	auto const pickupsCount = _pickupItems.Num();
	if (pickupsCount <= 0)
	{
		blackboardComponent->SetValueAsObject(_pickupActorKeyName, nullptr);
		return;
	}

	float closestDistance = INT_MAX;
	APickupItem* targetPickupItem = nullptr;

	for (auto const pickupItem : _pickupItems)
	{
		auto const canTakePickup = CanTakePickup(pickupItem, isNeedHealth, isNeedWeapon);
		if (!canTakePickup)
		{
			continue;
		}

		auto const world = GetWorld();
		auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(world);
		auto const pickupItemLocation = pickupItem->GetActorLocation();
		auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(world, selfLocation, pickupItemLocation);

		if (!navigationPath)
		{
			continue;
		}

		auto const navigationPathPointsCount = navigationPath->PathPoints.Num();

		if (navigationPathPointsCount < 2)
		{
			continue;
		}

		auto const pathLength = navigationPath->GetPathLength();

		if (closestDistance > pathLength)
		{
			closestDistance = pathLength;
			targetPickupItem = pickupItem;
		}
	}

	blackboardComponent->SetValueAsObject(_pickupActorKeyName, targetPickupItem);
}

AAvatar* UAvatarBTService::GetCurrentEnemy(
	UBlackboardComponent* blackboardComponent
)
{
	auto const enemyActorObject = blackboardComponent->GetValueAsObject(_enemyActorKeyName);
	if(!enemyActorObject)
	{
		return nullptr;
	}

	auto const enemyAvatar = Cast<AAvatar>(enemyActorObject);
	AssertIsNotNull(enemyAvatar, nullptr);

	auto const isAlive = IsAlive(enemyAvatar);

	return isAlive ? enemyAvatar : nullptr;
}

FVector UAvatarBTService::GetActorTraceLocation(AActor* thisActor) const
{
	auto const traceLocation = thisActor->GetActorLocation() + _avatarRayCastLocationOffset;
	return traceLocation;
}

bool UAvatarBTService::IsInLineOfSight(AActor* selfActor, AActor* otherActor)
{
	auto const thisActorLocation = GetActorTraceLocation(selfActor);
	auto const otherActorLocation = GetActorTraceLocation(otherActor);

	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(selfActor);
	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		thisActorLocation,
		otherActorLocation,
		_traceCollisionChannel,
		collisionQueryParams
	);

	//DrawDebugLine(GetWorld(), thisActorLocation, otherActorLocation, FColor::Blue, false, 1);
	auto const inLineOfSight = isHitted && otherActor == _hitResult.GetActor();

	return inLineOfSight;
}

bool UAvatarBTService::IsAlive(AActor* actor)
{
	auto const healthComponent = actor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent, false);

	return healthComponent->IsAlive();
}
