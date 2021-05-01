#include "MoveToTargetLocationBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UMoveToTargetLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();

	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const actor = Cast<AActor>(selfActorObject);
	auto const botMovementComponent = actor->FindComponentByClass<UBotMovementComponent>();
	
	auto const isTargetLocationValid = blackboardComponent->GetValueAsBool(_isTargetLocationValidKeyName);
	if(!isTargetLocationValid)
	{
		botMovementComponent->ClearTargetLocation();
		return EBTNodeResult::Failed;
	}

	auto const selfLocation = botMovementComponent->GetRootColliderLocation();
	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	
	auto const targetLocation = blackboardComponent->GetValueAsVector(_targetLocationKeyName);
	auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(GetWorld(), selfLocation, targetLocation);
	
	if(!navigationPath)
	{
		DevLogger::GetLoggingChannel()->Log("Navigation path is null", LogSeverity::Error);
		botMovementComponent->SetTargetLocation(targetLocation);
		return EBTNodeResult::Succeeded;
	}

	auto const navigationPathPointsCount = navigationPath->PathPoints.Num();
	
	if (navigationPathPointsCount <= 1)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Navigation path points count is invalid. Points count:",
			navigationPathPointsCount,
			LogSeverity::Error
		);
		botMovementComponent->SetTargetLocation(targetLocation);
		return EBTNodeResult::Succeeded;
	}
	
	int nearestPointIndex = -1;

	auto const locationToleranceSquared = blackboardComponent->GetValueAsFloat(_locationToleranceSquaredKeyName);
	
	for (auto i = 0; i < navigationPathPointsCount; i++)
	{
		auto pathPoint = navigationPath->PathPoints[i];
		//DrawDebugSphere(GetWorld(), pathPoint, 10.0f, 12, FColor::Green, false, 1, 0, 5);
		
		auto const distanceSquared = (pathPoint - selfLocation).SizeSquared2D();
		if(distanceSquared > locationToleranceSquared)
		{
			nearestPointIndex = i;
			break;
		}
	}

	if(nearestPointIndex == -1)
	{
		// Already at location
		botMovementComponent->ClearTargetLocation();
		return EBTNodeResult::Succeeded;
	}
	
	auto const nearestPoint = navigationPath->PathPoints[nearestPointIndex];
	botMovementComponent->SetTargetLocation(nearestPoint);

	return EBTNodeResult::Succeeded;
}
