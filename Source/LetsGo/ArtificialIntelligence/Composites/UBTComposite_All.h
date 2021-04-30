#pragma once

#include "BehaviorTree/BTCompositeNode.h"

#include "UBTComposite_All.generated.h"

/// <summary>
/// Executes all nodes regardless of result
/// </summary>
UCLASS()
class LETSGO_API UUBTComposite_All : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

protected:
	virtual int32 GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
};
