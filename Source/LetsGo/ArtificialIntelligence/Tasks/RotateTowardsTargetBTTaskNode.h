#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "RotateTowardsTargetBTTaskNode.generated.h"

UCLASS(Blueprintable)
class LETSGO_API URotateTowardsTargetBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _targetRotationKeyName = "TargetRotation";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isTargetRotationValidKeyName = "IsTargetRotationValid";
};
