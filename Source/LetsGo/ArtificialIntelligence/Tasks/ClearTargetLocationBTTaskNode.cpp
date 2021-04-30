#include "ClearTargetLocationBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UClearTargetLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);
	blackboardComponent->SetValueAsBool(_isTargetLocationValidKeyName, false);
	
	return EBTNodeResult::Succeeded;
}
