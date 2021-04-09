#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "LetsGo/GameModes/MatchGameMode.h"

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
	/// <summary>
	/// True [fast]: Pursue enemy till death. Checks if current enemy is alive, find next alive closest enemy if current is dead.
	/// False [slow]: Find closest enemy every tick.
	/// </summary>
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	bool _lockOnSingleEnemy;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TEnumAsByte<ECollisionChannel> _avatarRayCastCollisionChannel = ECC_Pawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FVector _avatarRayCastLocationOffset = FVector::UpVector * 100;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _enemyAvatarKeyName = "EnemyAvatar";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isEnemyInLineOfSightKeyName = "IsEnemyInLineOfSight";

	FHitResult _hitResult;
};
