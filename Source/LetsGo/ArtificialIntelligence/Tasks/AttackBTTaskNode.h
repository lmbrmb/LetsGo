#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "AttackBTTaskNode.generated.h"

/// <summary>
/// Attack task node. Aim and shoot.
/// </summary>
UCLASS()
class LETSGO_API UAttackBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _enemyAvatarKeyName = "EnemyAvatar";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FVector _avatarCenterOffset = FVector::UpVector * 100;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _minAimDotStartFire = 0.95f;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _aimOffsetRadius = 150;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _aimOffsetDistance = 1000;
	
	float _aimOffsetDistanceSquared = _aimOffsetDistance * _aimOffsetDistance;

	float GetRandomOffset() const;
};
