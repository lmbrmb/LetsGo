#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "SetTargetLocationAsRandomBTTaskNode.generated.h"

UCLASS()
class LETSGO_API USetTargetLocationAsRandomBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _randomLocationKeyName = "RandomLocation";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isRandomLocationValidKeyName = "IsRandomLocationValid";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _locationTolerance = 100;

	float _locationToleranceSquared = _locationTolerance * _locationTolerance;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Navigation)
	float _randomRadius = 3000;

	FNavLocation _navLocation;
};
