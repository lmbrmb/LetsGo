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
	
	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(selfActor);

	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		selfActor->GetActorLocation() + _avatarRayCastLocationOffset,
		enemyActor->GetActorLocation() + _avatarRayCastLocationOffset,
		_collisionChannel,
		collisionQueryParams
	);

	auto const isEnemyInLineOfSight = isHitted && enemyActor == _hitResult.GetActor();
	
	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	auto const rootColliderLocation = botMovementComponent->GetRootColliderLocation();

	auto const enemyLocation = enemyActor->GetActorLocation();
	auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(GetWorld(), rootColliderLocation, enemyLocation);

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
		auto const distanceSquared = FVector::DistSquared2D(pathPoint, rootColliderLocation);
		
		if (distanceSquared > _locationToleranceSquared)
		{
			nearestPointIndex = i;
			break;
		}
	}

	if (nearestPointIndex == -1)
	{
		// Already at location
		botMovementComponent->ClearTargetMovementLocation();
		return EBTNodeResult::Succeeded;
	}

	auto const enemyDistanceSquared = FVector::DistSquared2D(rootColliderLocation, enemyLocation);
	
	auto const nearestPoint = navigationPath->PathPoints[nearestPointIndex];
	botMovementComponent->SetTargetMovementLocation(nearestPoint);
	
	if (isEnemyInLineOfSight)
	{
		if(enemyDistanceSquared < _enemyDistanceSquared)
		{
			botMovementComponent->ClearTargetMovementLocation();
		}
		else
		{
			botMovementComponent->SetTargetMovementLocation(nearestPoint);
		}
	}
	else
	{
		botMovementComponent->SetTargetMovementLocation(nearestPoint);
	}

	return EBTNodeResult::Succeeded;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
EBTNodeResult::Type UMoveToEnemyBTTaskNode::TaskFailed(UBotMovementComponent* botMovementComponent)
{
	botMovementComponent->ClearTargetMovementLocation();
	return EBTNodeResult::Failed;
}
