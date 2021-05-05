#include "SetTargetLocationAsEnemyBTTaskNode.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

EBTNodeResult::Type USetTargetLocationAsEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);

	auto const selfBotMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	AssertIsNotNull(selfBotMovementComponent, EBTNodeResult::Failed);
	
	auto const enemyActorObject = blackboardComponent->GetValueAsObject(_enemyActorKeyName);
	if (!enemyActorObject)
	{
		selfBotMovementComponent->ClearTargetLocation();
		return EBTNodeResult::Failed;
	}

	auto const enemyActor = Cast<AActor>(enemyActorObject);
	AssertIsNotNull(enemyActor, EBTNodeResult::Failed);
	
	auto const enemyLocation = enemyActor->GetActorLocation();
	auto const selfLocation = selfActor->GetActorLocation();

	auto const isTargetLocationValid = selfBotMovementComponent->IsTargetLocationValid();
	if (isTargetLocationValid)
	{
		auto const targetLocation = selfBotMovementComponent->GetTargetLocation();
		auto const distanceSquared = FVector::DistSquared(selfLocation, targetLocation);

		//DrawDebugSphere(GetWorld(), targetLocation, 50, 10, FColor::Cyan, false, 0.25f);

		if (distanceSquared > _locationToleranceSquared)
		{
			// Not reached yet
			return EBTNodeResult::Succeeded;
		}
	}

	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	auto const directionFromEnemy = (selfLocation - enemyLocation).GetSafeNormal();
	auto const baseDistanceFromEnemy = GetBaseDistanceFromEnemy(enemyActor);
	auto targetLocation = enemyLocation + directionFromEnemy * baseDistanceFromEnemy;
	auto hasLocation = false;
	
	if (navigationSystemV1->GetRandomReachablePointInRadius(targetLocation, _baseRadiusAroundTargetLocation, _navLocation))
	{
		hasLocation = true;
	}
	else
	{
		for (auto i = 0; i < _triesToAdjustRadius; i++)
		{
			auto const radius = _baseRadiusAroundEnemy + i * _radiusIncrementPerTry;
			if (navigationSystemV1->GetRandomReachablePointInRadius(enemyLocation, radius, _navLocation))
			{
				hasLocation = true;
				break;
			}
		}
	}

	if (!hasLocation)
	{
		selfBotMovementComponent->ClearTargetLocation();
		return EBTNodeResult::Failed;
	}
	
	targetLocation = _navLocation.Location;
	selfBotMovementComponent->SetTargetLocation(targetLocation);

	return EBTNodeResult::Succeeded;
}

float USetTargetLocationAsEnemyBTTaskNode::GetBaseDistanceFromEnemy(AActor* selfActor)
{
	AssertIsNotNull(selfActor, _defaultBaseDistanceFromEnemy);
	
	auto const selfWeaponManger = selfActor->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(selfWeaponManger, _defaultBaseDistanceFromEnemy);

	auto const currentWeaponId = selfWeaponManger->GetCurrentWeaponId();
	if(_weaponRanges.Contains(currentWeaponId))
	{
		return _weaponRanges[currentWeaponId];
	}
	
	return _defaultBaseDistanceFromEnemy;
}
