#include "AvatarBTService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

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

	auto const healthComponent = selfAvatar->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	auto const weaponManagerComponent = selfAvatar->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManagerComponent);
	
	_enemies.Empty();
	_pickupItems.Empty();

	auto const selfLocation = selfAvatar->GetActorLocation();
	auto const needHealth = IsNeedHealth(healthComponent);
	auto const needWeapon = IsNeedWeapon(weaponManagerComponent);
	
	Detect(selfAvatar, selfLocation, needHealth, needWeapon);
	UpdatePickup(blackboardComponent, needHealth, needWeapon, selfLocation);
	UpdateEnemy(blackboardComponent, selfAvatar, selfLocation);
}

bool UAvatarBTService::IsNeedHealth(UHealthComponent* healthComponent)
{
	auto const currentHealth = healthComponent->GetCurrentValue();
	auto const maxNormalHealth = healthComponent->GetMaxNormalHealth();
	auto const needHealth = currentHealth < maxNormalHealth;
	return needHealth;
}

bool UAvatarBTService::IsNeedWeapon(UWeaponManagerComponent* weaponManagerComponent)
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

bool UAvatarBTService::IsEnoughAmmo(IGun* gun)
{
	auto const ammoProvider = gun->GetAmmoProvider();

	auto const maxAmmo = ammoProvider->GetMax();
	AssertIsGreater(maxAmmo, 0, false);

	auto const ammoPercent = ammoProvider->GetCurrent() / maxAmmo;
	return ammoPercent >= _minAmmoPercent;
}

void UAvatarBTService::Detect(
	AAvatar* selfAvatar,
	const FVector& selfLocation,
	const bool needHealth,
	const bool needWeapon
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

		if(TryProcessPickup(needHealth, needWeapon, hitActor))
		{
			continue;
		}
		
		TryProcessEnemy(hitActor, selfTeamId);
	}
}

bool UAvatarBTService::TryProcessEnemy(
	AActor* otherActor,
	const TeamId& selfTeamId
)
{
	auto const avatar = Cast<AAvatar>(otherActor);
	
	if (!avatar)
	{
		return false;
	}

	if (avatar->GetTeamId() == selfTeamId)
	{
		return false;
	}

	auto const isAlive = IsAlive(avatar);

	if(!isAlive)
	{
		return false;
	}
	
	_enemies.Add(avatar);
	return true;
}

bool UAvatarBTService::TryProcessPickup(
	const bool needHealth,
	const bool needWeapon,
	AActor* otherActor
)
{
	auto const pickupItem = Cast<APickupItem>(otherActor);

	if (!pickupItem)
	{
		return false;
	}

	auto const canTakePickup = CanTakePickup(needHealth, needWeapon, pickupItem);

	if(!canTakePickup)
	{
		
		return false;
	}

	_pickupItems.Add(pickupItem);
	return true;
}

bool UAvatarBTService::CanTakePickup(
	const bool needHealth,
	const bool needWeapon,
	const APickupItem* pickupItem
) const
{
	auto const pickupId = pickupItem->GetId();

	if (pickupItem->IsTaken())
	{
		return false;
	}

	if (_weaponPickups.Contains(pickupId))
	{
		return true;
	}
	
	if(_superHealthPickups.Contains(pickupId))
	{
		return true;
	}

	if (_normalHealthPickups.Contains(pickupId))
	{
		return needHealth;
	}
	
	return false;
}

void UAvatarBTService::UpdateEnemy(
	UBlackboardComponent* blackboardComponent,
	AAvatar* selfAvatar,
	const FVector& selfLocation
)
{
	if (_enemies.Num() <= 0)
	{
		blackboardComponent->SetValueAsObject(_enemyActorKeyName, nullptr);
		return;
	}

	AActor* closestEnemy = nullptr;
	float closestDistance = INT_MAX;
	
	for (auto const enemy : _enemies)
	{
		auto const squaredDistance = (enemy->GetActorLocation() - selfLocation).SizeSquared();

		if (closestDistance > squaredDistance)
		{
			closestDistance = squaredDistance;
			closestEnemy = enemy;
		}
	}

	AssertIsNotNull(closestEnemy);
	blackboardComponent->SetValueAsObject(_enemyActorKeyName, closestEnemy);
	auto const isEnemyInLineOfSight = IsInLineOfSight(selfAvatar, closestEnemy);
	blackboardComponent->SetValueAsBool(_isEnemyInLineOfSightKeyName, isEnemyInLineOfSight);
}

void UAvatarBTService::UpdatePickup(
	UBlackboardComponent* blackboardComponent,
	const bool needHealth,
	const bool needWeapon,
	const FVector& selfLocation
) const
{
	auto const currentPickupActor = blackboardComponent->GetValueAsObject(_pickupActorKeyName);
	if(currentPickupActor)
	{
		auto const pickupItem = Cast<APickupItem>(currentPickupActor);
		AssertIsNotNull(pickupItem);
		auto const canTakePickup = CanTakePickup(needHealth, needWeapon, pickupItem);
		if(canTakePickup)
		{
			return;
		}
	}

	if (_pickupItems.Num() <= 0)
	{
		blackboardComponent->SetValueAsObject(_pickupActorKeyName, nullptr);
		return;
	}

	float closestDistance = INT_MAX;
	APickupItem* closestPickupItem = nullptr;

	for (auto const pickupItem : _pickupItems)
	{
		auto const squaredDistance = (pickupItem->GetActorLocation() - selfLocation).SizeSquared();

		if (closestDistance > squaredDistance)
		{
			closestDistance = squaredDistance;
			closestPickupItem = pickupItem;
		}
	}
	blackboardComponent->SetValueAsObject(_pickupActorKeyName, closestPickupItem);
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

bool UAvatarBTService::IsInLineOfSight(AActor* leftActor, AActor* rightActor)
{
	auto const thisActorLocation = GetActorTraceLocation(leftActor);
	auto const otherActorLocation = GetActorTraceLocation(rightActor);

	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(leftActor);
	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		thisActorLocation,
		otherActorLocation,
		_traceCollisionChannel,
		collisionQueryParams
	);

	//DrawDebugLine(GetWorld(), thisActorLocation, otherActorLocation, FColor::Blue, false, 1);
	auto const inLineOfSight = isHitted && rightActor == _hitResult.GetActor();

	return inLineOfSight;
}

bool UAvatarBTService::IsAlive(AActor* actor)
{
	auto const healthComponent = actor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent, false);

	return healthComponent->IsAlive();
}
