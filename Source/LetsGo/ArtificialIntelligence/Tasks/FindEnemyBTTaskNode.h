#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "LetsGo/GameModes/MatchGameMode.h"

#include "FindEnemyBTTaskNode.generated.h"

/// <summary>
/// Find enemy task node
/// </summary>
UCLASS()
class LETSGO_API UFindEnemyBTTaskNode : public UBTTaskNode
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
};
