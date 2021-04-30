#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "LetsGo/Movement/BotMovementComponent.h"


#include "DebugPrintTextBTTaskNode.generated.h"

/// <summary>
/// Debug print text task node
/// </summary>
UCLASS(Blueprintable)
class LETSGO_API UDebugPrintTextBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FString _text;
};
