#include "UBTComposite_All.h"

#include "BehaviorTree/BTCompositeNode.h"

UUBTComposite_All::UUBTComposite_All(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "All";
}

int32 UUBTComposite_All::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild,
	EBTNodeResult::Type LastResult) const
{
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		NextChildIdx = 0;
	}
	else if (PrevChild + 1 < GetChildrenNum())
	{
		NextChildIdx = PrevChild + 1;
	}

	return NextChildIdx;
}
