#include "DebugPrintTextBTTaskNode.h"

#include "LetsGo/Logs/DevLogger.h"

EBTNodeResult::Type UDebugPrintTextBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	DevLogger::GetLoggingChannel()->Log(_text);
	return EBTNodeResult::Succeeded;
}
