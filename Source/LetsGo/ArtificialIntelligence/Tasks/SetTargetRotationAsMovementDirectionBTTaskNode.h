#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "SetTargetRotationAsMovementDirectionBTTaskNode.generated.h"

UCLASS(Blueprintable)
class LETSGO_API USetTargetRotationAsMovementDirectionBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _targetLocationKeyName = "TargetLocation";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isTargetLocationValidKeyName = "IsTargetLocationValid";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _targetRotationKeyName = "TargetRotation";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isTargetRotationValidKeyName = "IsTargetRotationValid";
};
