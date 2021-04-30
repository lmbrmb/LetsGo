#include "SetTargetLocationAsPickupBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetLocationAsPickupBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const pickupActorObject = blackboardComponent->GetValueAsObject(_pickupActorKeyName);

	if (!pickupActorObject)
	{
		return EBTNodeResult::Failed;
	}

	auto const pickupActor = Cast<AActor>(pickupActorObject);
	AssertIsNotNull(pickupActor, EBTNodeResult::Failed);

	auto const enemyLocation = pickupActor->GetActorLocation();
	blackboardComponent->SetValueAsVector(_targetLocationKeyName, enemyLocation);
	blackboardComponent->SetValueAsBool(_isTargetLocationValidKeyName, true);
	blackboardComponent->SetValueAsFloat(_locationToleranceSquaredKeyName, _locationToleranceSquared);
	
	return EBTNodeResult::Succeeded;
}
