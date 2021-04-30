#include "RotateTowardsTargetBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Movement/BotMovementComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type URotateTowardsTargetBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	auto const selfBotMovementComponent = selfActor->FindComponentByClass<UBotMovementComponent>();
	AssertIsNotNull(selfBotMovementComponent, EBTNodeResult::Failed);
	
	auto const isTargetRotationValid = blackboardComponent->GetValueAsBool(_isTargetRotationValidKeyName);

	if(isTargetRotationValid)
	{
		auto const targetRotation = blackboardComponent->GetValueAsVector(_targetRotationKeyName);
		selfBotMovementComponent->SetTargetRotation(targetRotation);
	}
	else
	{
		
		selfBotMovementComponent->ClearTargetRotation();
	}

	return EBTNodeResult::Succeeded;
}
