#pragma once

#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"

#include "BotAiController.generated.h"

/// <summary>
/// Bot artificial intelligence controller
/// </summary>
UCLASS()
class LETSGO_API ABotAiController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	UBehaviorTree* _behaviorTree;
};
