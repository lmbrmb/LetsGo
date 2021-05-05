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

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _locationTolerance = 100;

	float _locationToleranceSquared = _locationTolerance * _locationTolerance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _baseRadiusAroundTargetLocation = 750;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _baseRadiusAroundEnemy = 500;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _defaultBaseDistanceFromEnemy = 1000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	int _triesToAdjustRadius = 3;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _radiusIncrementPerTry = 300;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	TMap<FName, float> _weaponRanges;
	
	FNavLocation _navLocation;

	float GetBaseDistanceFromEnemy(AActor* selfActor);
};
