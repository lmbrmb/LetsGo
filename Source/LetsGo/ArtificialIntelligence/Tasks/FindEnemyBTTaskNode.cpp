#include "FindEnemyBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UFindEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();

	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);
	
	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const actor = Cast<AActor>(selfActorObject);
	
	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);

	//TEST:
	auto const enemyLocation = FVector(-640, -90, 120);
	blackboardComponent->SetValueAsBool(_isTargetLocationValidKeyName, true);
	blackboardComponent->SetValueAsVector(_targetLocationKeyName, enemyLocation);
	
	return EBTNodeResult::Succeeded;
}
