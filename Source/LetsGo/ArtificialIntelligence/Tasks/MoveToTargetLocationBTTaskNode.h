#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "LetsGo/Movement/BotMovementComponent.h"

#include "MoveToTargetLocationBTTaskNode.generated.h"

/// <summary>
/// AI - "Move to" task node
/// </summary>
UCLASS(Blueprintable)
class LETSGO_API UMoveToTargetLocationBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _targetLocationKeyName = "TargetLocation";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isTargetLocationValidKeyName = "IsTargetLocationValid";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _locationToleranceSquaredKeyName = "LocationToleranceSquared";
};
