#include "SetTargetLocationAsEnemyBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetLocationAsEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);
	
	auto const enemyActorObject = blackboardComponent->GetValueAsObject(_enemyActorKeyName);

	if(!enemyActorObject)
	{
		return EBTNodeResult::Failed;
	}

	auto const enemyActor = Cast<AActor>(enemyActorObject);
	AssertIsNotNull(enemyActor, EBTNodeResult::Failed);
	
	auto const enemyLocation = enemyActor->GetActorLocation();
	blackboardComponent->SetValueAsVector(_targetLocationKeyName, enemyLocation);
	blackboardComponent->SetValueAsBool(_isTargetLocationValidKeyName, true);
	blackboardComponent->SetValueAsFloat(_locationToleranceSquaredKeyName, _locationToleranceSquared);
	
	return EBTNodeResult::Succeeded;
}
