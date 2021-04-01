#include "MoveToBTTaskNode.h"

#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Logs/DevLogger.h"

EBTNodeResult::Type UMoveToBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!blackboardComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No blackboard", LogSeverity::Error);
		return EBTNodeResult::Failed;
	}

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const actor = Cast<AActor>(selfActorObject);
	auto const movementComponent = actor->FindComponentByClass<UBotMovementComponent>();

	auto const targetLocation = FVector(-640, -90, 120);
	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
	auto const rootColliderLocation = movementComponent->GetRootColliderLocation();
	auto rootColliderLocationNoZ = rootColliderLocation;
	rootColliderLocationNoZ.Z = 0;
	
	auto const navigationPath = navigationSystemV1->FindPathToLocationSynchronously(GetWorld(), rootColliderLocation, targetLocation);

	if(!navigationPath)
	{
		DevLogger::GetLoggingChannel()->Log("Navigation path is null", LogSeverity::Error);
		return EBTNodeResult::Failed;
	}

	auto const navigationPathPointsCount = navigationPath->PathPoints.Num();

	if (navigationPathPointsCount <= 1)
	{
		DevLogger::GetLoggingChannel()->Log("Navigation path is invalid", LogSeverity::Error);
		return EBTNodeResult::Failed;
	}
	
	int nearestPointIndex = -1;
	
	//First point is actor's location, so start checking with second
	for (int i = 1; i < navigationPathPointsCount; i++)
	{
		auto pathPoint = navigationPath->PathPoints[i];
		//DrawDebugSphere(GetWorld(), pathPoint, 10.0f, 12, FColor::Green, false, -1, 0, 5);
		// Z is ignored
		pathPoint.Z = 0;
		//SizeSquared is optimization
		auto const distanceSquared = (pathPoint - rootColliderLocationNoZ).SizeSquared();
		if(distanceSquared > _targetLocationToleranceSquared)
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
	movementComponent->SetTargetLocation(nearestPoint);

	return EBTNodeResult::Succeeded;
}
