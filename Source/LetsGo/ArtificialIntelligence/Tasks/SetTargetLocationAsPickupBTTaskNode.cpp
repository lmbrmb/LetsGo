#include "SetTargetLocationAsPickupBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Movement/BotMovementComponent.h"
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

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);

	auto const selfBotMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	AssertIsNotNull(selfBotMovementComponent, EBTNodeResult::Failed);

	auto const pickupLocation = pickupActor->GetActorLocation();
	selfBotMovementComponent->SetTargetLocation(pickupLocation);

	return EBTNodeResult::Succeeded;
}
