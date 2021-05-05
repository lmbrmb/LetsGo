#include "SetTargetLocationAsEnemyBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

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
	if(!enemyActorObject)
	{
		selfBotMovementComponent->ClearTargetLocation();
		return EBTNodeResult::Failed;
	}

	auto const enemyActor = Cast<AActor>(enemyActorObject);
	AssertIsNotNull(enemyActor, EBTNodeResult::Failed);

	auto const enemyLocation = enemyActor->GetActorLocation();
	selfBotMovementComponent->SetTargetLocation(enemyLocation);

	return EBTNodeResult::Succeeded;
}
