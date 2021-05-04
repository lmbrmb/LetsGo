#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "AimAndFireBTTaskNode.generated.h"

UCLASS()
class LETSGO_API UAimAndFireBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _enemyActorKeyName = "EnemyActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isEnemyInLineOfSightKeyName = "IsEnemyInLineOfSight";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Aim)
	FVector _avatarCenterOffset = FVector::UpVector * 50;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Aim)
	float _minAimDotStartFire = 0.98f;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Aim)
	float _aimOffsetRadius = 100;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Aim)
	float _aimOffsetDistance = 1000;
	
	float _aimOffsetDistanceSquared = _aimOffsetDistance * _aimOffsetDistance;

	float GetRandomOffset() const;
};
