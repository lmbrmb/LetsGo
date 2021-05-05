#include "SetTargetLocationAsRandomBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetLocationAsRandomBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);

	auto const selfBotMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	AssertIsNotNull(selfBotMovementComponent, EBTNodeResult::Failed);

	auto const selfLocation = selfActor->GetActorLocation();

	auto const isTargetLocationValid = selfBotMovementComponent->IsTargetLocationValid();
	if(isTargetLocationValid)
	{
		auto const targetLocation = selfBotMovementComponent->GetTargetLocation();
		auto const distanceSquared = FVector::DistSquared(selfLocation, targetLocation);

		//DrawDebugSphere(GetWorld(), targetLocation, 50, 10, FColor::Red, false, 0.25f);
		
		if(distanceSquared > _locationToleranceSquared)
		{
			// Not reached yet
			return EBTNodeResult::Succeeded;
		}
	}

	auto const navigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());

	if(!navigationSystemV1->GetRandomReachablePointInRadius(selfLocation, _randomRadius, _navLocation))
	{
		selfBotMovementComponent->ClearTargetLocation();
		return EBTNodeResult::Failed;
	}

	auto const randomLocation = _navLocation.Location;
	selfBotMovementComponent->SetTargetLocation(randomLocation);

	return EBTNodeResult::Succeeded;
}
