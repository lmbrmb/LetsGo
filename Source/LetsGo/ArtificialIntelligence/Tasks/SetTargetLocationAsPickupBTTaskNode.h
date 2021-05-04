#pragma once

#include "BehaviorTree/BTTaskNode.h"

#include "SetTargetLocationAsPickupBTTaskNode.generated.h"

UCLASS()
class LETSGO_API USetTargetLocationAsPickupBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _pickupActorKeyName = "PickupActor";
};
