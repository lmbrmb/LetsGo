#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "ClearTargetRotationBTTaskNode.generated.h"

UCLASS(Blueprintable)
class LETSGO_API UClearTargetRotationBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";
};
