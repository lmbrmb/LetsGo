#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "FindWeaponBTTaskNode.generated.h"

/// <summary>
/// Find weapon task node
/// </summary>
UCLASS()
class LETSGO_API UFindWeaponBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
