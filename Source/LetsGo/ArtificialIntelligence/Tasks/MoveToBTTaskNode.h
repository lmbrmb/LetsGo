#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "MoveToBTTaskNode.generated.h"

/// <summary>
/// AI - "Move to" task node
/// </summary>
UCLASS(Blueprintable)
class LETSGO_API UMoveToBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _selfActorKeyName = "SelfActor";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _targetLocationKeyName = "TargetLocation";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _isTargetLocationValidKeyName = "IsTargetLocationValid";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _targetLocationTolerance = 10;

	float _targetLocationToleranceSquared = _targetLocationTolerance * _targetLocationTolerance;
};
