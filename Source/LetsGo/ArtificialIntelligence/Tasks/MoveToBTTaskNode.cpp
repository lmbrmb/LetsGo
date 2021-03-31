#include "MoveToBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Movement/MovementComponentBase.h"
#include "LetsGo/Logs/DevLogger.h"

EBTNodeResult::Type UMoveToBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!blackboardComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No blackboard", LogSeverity::Error);
		return EBTNodeResult::Failed;
	}

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const actor = Cast<AActor>(selfActorObject);
	auto const movementComponent = actor->FindComponentByClass<UMovementComponentBase>();
	
	return EBTNodeResult::Succeeded;
}
