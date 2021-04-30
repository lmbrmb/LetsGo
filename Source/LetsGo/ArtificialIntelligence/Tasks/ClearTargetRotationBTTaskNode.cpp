#include "ClearTargetRotationBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UClearTargetRotationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	blackboardComponent->SetValueAsBool(_isTargetRotationValidKeyName, false);

	return EBTNodeResult::Succeeded;
}
