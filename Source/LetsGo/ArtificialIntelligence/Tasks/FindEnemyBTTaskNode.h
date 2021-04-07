#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "LetsGo/GameModes/MatchGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "FindEnemyBTTaskNode.generated.h"

/// <summary>
/// Find enemy task node. Algorithm traits:
/// 1. Enemy = closest alive non-teammate avatar
/// 2. Check enemy health
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
	FName _enemyAvatarKeyName = "EnemyAvatar";

	/// <summary>
	/// True [fast]: Pursue enemy till death. Checks if current enemy is alive, find next alive closest enemy if current is dead.
	/// False [slow]: Find closest enemy every tick.
	/// </summary>
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _lockOnSingleEnemy;
	
	EBTNodeResult::Type TaskFailed(UBlackboardComponent* blackboardComponent);
};
