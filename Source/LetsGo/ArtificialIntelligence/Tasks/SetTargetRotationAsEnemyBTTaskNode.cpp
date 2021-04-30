#include "SetTargetRotationAsEnemyBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetRotationAsEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const enemyActorObject = blackboardComponent->GetValueAsObject(_enemyActorKeyName);
	if (!enemyActorObject)
	{
		return EBTNodeResult::Failed;
	}

	auto const enemyActor = Cast<AActor>(enemyActorObject);
	AssertIsNotNull(enemyActor, EBTNodeResult::Failed);
	
	auto const enemyLocation = enemyActor->GetActorLocation();
	blackboardComponent->SetValueAsVector(_targetRotationKeyName, enemyLocation);
	blackboardComponent->SetValueAsBool(_isTargetRotationValidKeyName, true);

	return EBTNodeResult::Succeeded;
}
