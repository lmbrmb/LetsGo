#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "SetTargetLocationAsEnemyBTTaskNode.generated.h"

UCLASS()
class LETSGO_API USetTargetLocationAsEnemyBTTaskNode : public UBTTaskNode
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
	FName _targetLocationKeyName = "TargetLocation";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isTargetLocationValidKeyName = "IsTargetLocationValid";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _locationToleranceSquaredKeyName = "LocationToleranceSquared";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _locationTolerance = 500;

	float _locationToleranceSquared = _locationTolerance * _locationTolerance;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _enemyDistance = 700;
	
	float _enemyDistanceSquared = _enemyDistance * _enemyDistance;
};
