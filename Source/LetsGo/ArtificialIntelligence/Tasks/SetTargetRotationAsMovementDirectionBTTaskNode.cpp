#include "SetTargetRotationAsMovementDirectionBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type USetTargetRotationAsMovementDirectionBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const isTargetLocationValid = blackboardComponent->GetValueAsBool(_targetLocationKeyName);
	if(!isTargetLocationValid)
	{
		return EBTNodeResult::Failed;
	}
	
	auto const targetLocation = blackboardComponent->GetValueAsVector(_targetLocationKeyName);
	
	blackboardComponent->SetValueAsVector(_targetRotationKeyName, targetLocation);
	blackboardComponent->SetValueAsBool(_isTargetRotationValidKeyName, true);

	return EBTNodeResult::Succeeded;
}
