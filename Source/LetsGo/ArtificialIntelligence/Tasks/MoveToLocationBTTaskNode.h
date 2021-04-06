#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "LetsGo/Movement/BotMovementComponent.h"


#include "MoveToLocationBTTaskNode.generated.h"

/// <summary>
/// AI - "Move to" task node
/// </summary>
UCLASS(Blueprintable)
class LETSGO_API UMoveToLocationBTTaskNode : public UBTTaskNode
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
	float _locationTolerance = 10;

	float _locationToleranceSquared = _locationTolerance * _locationTolerance;

	EBTNodeResult::Type TaskFailed(UBotMovementComponent* botMovementComponent);
};
