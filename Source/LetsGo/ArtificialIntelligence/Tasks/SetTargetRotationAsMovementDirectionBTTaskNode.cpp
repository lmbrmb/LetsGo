#include "SetTargetRotationAsMovementDirectionBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetRotationAsMovementDirectionBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);

	auto const selfBotMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	AssertIsNotNull(selfBotMovementComponent, EBTNodeResult::Failed);

	selfBotMovementComponent->SetTargetRotationAsMovementDirection();

	return EBTNodeResult::Succeeded;
}
