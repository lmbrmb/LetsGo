#include "SetTargetRotationAsEnemyBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetRotationAsEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const enemyActorObject = blackboardComponent->GetValueAsObject(_enemyActorKeyName);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);

	auto const selfBotMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	AssertIsNotNull(selfBotMovementComponent, EBTNodeResult::Failed);

	if(!enemyActorObject)
	{
		selfBotMovementComponent->ClearTargetRotation();
		return EBTNodeResult::Failed;
	}

	auto const enemyActor = Cast<AActor>(enemyActorObject);
	AssertIsNotNull(enemyActor, EBTNodeResult::Failed);

	auto const enemyLocation = enemyActor->GetActorLocation();
	selfBotMovementComponent->SetTargetRotationAsLocation(enemyLocation);

	return EBTNodeResult::Succeeded;
}
