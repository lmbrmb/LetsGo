#include "MoveToLocationBTTaskNode.h"

#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UMoveToLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();

	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const actor = Cast<AActor>(selfActorObject);
	auto const botMovementComponent = actor->FindComponentByClass<UBotMovementComponent>();
	
	auto const isTargetLocationValid = blackboardComponent->GetValueAsBool(_isTargetLocationValidKeyName);
	if(!isTargetLocationValid)
	{
		return TaskFailed(botMovementComponent);
	}
	
	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	auto const rootColliderLocation = botMovementComponent->GetRootColliderLocation();
	
	auto const targetLocation = blackboardComponent->GetValueAsVector(_targetLocationKeyName);
	auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(GetWorld(), rootColliderLocation, targetLocation);
	
	if(!navigationPath)
	{
		DevLogger::GetLoggingChannel()->Log("Navigation path is null", LogSeverity::Error);
		return TaskFailed(botMovementComponent);
	}

	auto const navigationPathPointsCount = navigationPath->PathPoints.Num();
	
	if (navigationPathPointsCount <= 1)
	{
		DevLogger::GetLoggingChannel()->Log("Navigation path points count is invalid", LogSeverity::Error);
		return EBTNodeResult::Failed;
	}
	
	int nearestPointIndex = -1;
	
	for (auto i = 0; i < navigationPathPointsCount; i++)
	{
		auto pathPoint = navigationPath->PathPoints[i];
		//DrawDebugSphere(GetWorld(), pathPoint, 10.0f, 12, FColor::Green, false, -1, 0, 5);
		
		auto const distanceSquared = (pathPoint - rootColliderLocation).SizeSquared2D();
		if(distanceSquared > _locationToleranceSquared)
		{
			nearestPointIndex = i;
			break;
		}
	}

	if(nearestPointIndex == -1)
	{
		// Already at location
		return EBTNodeResult::Succeeded;
	}
	
	auto const nearestPoint = navigationPath->PathPoints[nearestPointIndex];
	botMovementComponent->SetTargetMovementLocation(nearestPoint);

	return EBTNodeResult::Succeeded;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
EBTNodeResult::Type UMoveToLocationBTTaskNode::TaskFailed(UBotMovementComponent* botMovementComponent)
{
	botMovementComponent->ClearTargetMovementLocation();
	return EBTNodeResult::Failed;
}
