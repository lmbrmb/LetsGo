#include "AvatarBTService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/ActorUtils.h"
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
	
	TArray<AAvatar*> enemies;
	TArray<APickupItem*> pickupItems;

	auto const selfLocation = selfAvatar->GetActorLocation();
	Detect(blackboardComponent, selfAvatar, healthComponent, selfLocation, enemies, pickupItems);
	UpdateEnemy(blackboardComponent, selfAvatar, selfLocation, enemies);
	UpdatePickup(blackboardComponent, selfLocation, pickupItems);
}

void UAvatarBTService::UpdateWeapon(UBlackboardComponent* blackboardComponent, AActor* selfActor)
{
	auto const weaponManagerComponent = selfActor->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManagerComponent);
	
	auto const shouldChangeWeapon = ShouldChangeWeapon(weaponManagerComponent);
	blackboardComponent->SetValueAsBool(_shouldChangeWeaponKeyName, shouldChangeWeapon);

	if(shouldChangeWeapon)
	{
		auto const canChangeWeapon = CanChangeWeapon(weaponManagerComponent);
		blackboardComponent->SetValueAsBool(_canChangeWeaponKeyName, canChangeWeapon);
	}
}

bool UAvatarBTService::ShouldChangeWeapon(UWeaponManagerComponent* weaponManagerComponent)
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

	auto const currentGun = weaponManagerComponent->GetCurrentGun();
	if (!currentGun)
	{
		return true;
	}

	auto const isEnoughAmmo = IsEnoughAmmo(currentGun);
	return !isEnoughAmmo;
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

bool UAvatarBTService::CanChangeWeapon(UWeaponManagerComponent* weaponManagerComponent)
{
	auto const weaponsCount = weaponManagerComponent->GetWeaponsCount();
	if(weaponsCount <= 1)
	{
		return false;
	}

	for (auto const gun : weaponManagerComponent->GetGuns())
	{
		auto const isLowTierWeapon = IsLowTierWeapon(gun);
		if(isLowTierWeapon)
		{
			continue;
		}

		auto const isEnoughAmmo = IsEnoughAmmo(gun);
		if(!isEnoughAmmo)
		{
			continue;
		}

		return true;
	}

	return false;
}

void UAvatarBTService::Detect(
	UBlackboardComponent* blackboardComponent,
	AAvatar* selfAvatar,
	UHealthComponent* healthComponent,
	const FVector& selfLocation,
	TArray<AAvatar*>& enemies,
	TArray<APickupItem*>& pickupItems
)
{
	auto const currentHealth = healthComponent->GetCurrentValue();
	auto const maxNormalHealth = healthComponent->GetMaxNormalHealth();
	auto const isHealthBelowNormal = currentHealth < maxNormalHealth;
	
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

		if(TryProcessPickup(isHealthBelowNormal, hitActor, pickupItems))
		{
			continue;
		}
		
		TryProcessEnemy(hitActor, selfTeamId, enemies);
	}
}

bool UAvatarBTService::TryProcessEnemy(
	AActor* otherActor,
	const TeamId& selfTeamId,
	TArray<AAvatar*>& enemies
) const
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
	
	enemies.Add(avatar);
	return true;
}

bool UAvatarBTService::TryProcessPickup(
	const bool isHealthBelowNormal,
	AActor* otherActor,
	TArray<APickupItem*>& pickupItems
) const
{
	auto const pickupItem = Cast<APickupItem>(otherActor);

	if (!pickupItem)
	{
		return false;
	}

	auto const canTakePickup = CanTakePickup(isHealthBelowNormal, pickupItem);

	if(!canTakePickup)
	{
		
		return false;
	}

	pickupItems.Add(pickupItem);
	return true;
}

bool UAvatarBTService::CanTakePickup(
	const bool isHealthBelowNormal,
	const APickupItem* pickupItem
) const
{
	auto const pickupId = pickupItem->GetId();

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
		return isHealthBelowNormal;
	}
	
	return false;
}

void UAvatarBTService::UpdateEnemy(
	UBlackboardComponent* blackboardComponent,
	AAvatar* selfAvatar,
	const FVector& selfLocation,
	TArray<AAvatar*>& enemies
)
{
	if (enemies.Num() <= 0)
	{
		blackboardComponent->SetValueAsObject(_enemyActorKeyName, nullptr);
		return;
	}

	AActor* closestEnemy = nullptr;
	float closestDistance = INT_MAX;
	
	for (auto const enemy : enemies)
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
	const FVector& selfLocation,
	TArray<APickupItem*>& pickupItems
) const
{
	if (pickupItems.Num() <= 0)
	{
		blackboardComponent->SetValueAsObject(_pickupActorKeyName, nullptr);
		return;
	}

	float closestDistance = INT_MAX;
	APickupItem* closestPickupItem = nullptr;

	for (auto const pickupItem : pickupItems)
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
