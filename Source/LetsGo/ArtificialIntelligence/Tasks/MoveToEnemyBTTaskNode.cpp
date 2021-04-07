#include "MoveToEnemyBTTaskNode.h"

#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UMoveToEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);
	
	auto const botMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	auto const enemyActorObject = blackboardComponent->GetValueAsObject(_enemyAvatarKeyName);
	AssertIsNotNull(enemyActorObject, EBTNodeResult::Failed);

	auto const enemyActor = Cast<AActor>(enemyActorObject);
	AssertIsNotNull(enemyActor, EBTNodeResult::Failed);

	auto const enemyLocation = enemyActor->GetActorLocation();
	auto const selfActorLocation	= botMovementComponent->GetRootColliderLocation();
	
	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(GetWorld(), selfActorLocation, enemyLocation);

	if (!navigationPath)
	{
		DevLogger::GetLoggingChannel()->Log("Navigation path is null", LogSeverity::Error);
		return TaskFailed(botMovementComponent);
	}
	
	auto const navigationPathPointsCount = navigationPath->PathPoints.Num();
	int nearestPointIndex = -1;
	
	for (auto i = 0; i < navigationPathPointsCount; i++)
	{
		auto pathPoint = navigationPath->PathPoints[i];
		//DrawDebugSphere(GetWorld(), pathPoint, 10.0f, 12, FColor::Green, false, -1, 0, 5);
		auto const distanceSquared = FVector::DistSquared2D(pathPoint, selfActorLocation);
		
		if (distanceSquared > _locationToleranceSquared)
		{
			nearestPointIndex = i;
			break;
		}
	}

	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(selfActor);
	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		selfActorLocation + _avatarRayCastLocationOffset,
		enemyLocation + _avatarRayCastLocationOffset,
		_collisionChannel,
		collisionQueryParams
	);
	auto const isEnemyInLineOfSight = isHitted && enemyActor == _hitResult.GetActor();
	
	if (nearestPointIndex == -1)
	{
		botMovementComponent->ClearTargetLocation();

		if (isEnemyInLineOfSight)
		{
			botMovementComponent->SetTargetRotation(enemyLocation);
		}
		else
		{
			botMovementComponent->ClearTargetRotation();
		}
		
		return EBTNodeResult::Succeeded;
	}
	
	auto const enemyDistanceSquared = FVector::DistSquared2D(selfActorLocation, enemyLocation);
	
	if (isEnemyInLineOfSight)
	{
		botMovementComponent->SetTargetRotation(enemyLocation);
		
		if( enemyDistanceSquared < _enemyDistanceSquared)
		{
			botMovementComponent->ClearTargetLocation();
		}
		else
		{
			auto const nearestPoint = navigationPath->PathPoints[nearestPointIndex];
			botMovementComponent->SetTargetLocation(nearestPoint);
		}
	}
	else
	{
		auto const nearestPoint = navigationPath->PathPoints[nearestPointIndex];
		botMovementComponent->SetTargetRotation(nearestPoint);
		botMovementComponent->SetTargetLocation(nearestPoint);
	}
	
	return EBTNodeResult::Succeeded;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
EBTNodeResult::Type UMoveToEnemyBTTaskNode::TaskFailed(UBotMovementComponent* botMovementComponent)
{
	botMovementComponent->ClearTargetLocation();
	botMovementComponent->ClearTargetRotation();
	return EBTNodeResult::Failed;
}
