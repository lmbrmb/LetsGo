#include "FindEnemyBTTaskNode.h"

#include "LetsGo/Utils/AssertUtils.h"

EBTNodeResult::Type UFindEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);
	
	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	auto const selfAvatar = Cast<AAvatar>(selfActorObject);
	AssertIsNotNull(selfAvatar, EBTNodeResult::Failed);

	auto const selfTeamId = selfAvatar->GetTeamId();
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);

	float closestDistance = MAX_int32;
	auto const selfLocation = selfAvatar->GetActorLocation();
	AAvatar* closestEnemyAvatar = nullptr;
	
	for (auto avatar : matchGameMode->GetAvatars())
	{
		if(avatar == selfAvatar)
		{
			continue;
		}

		if(avatar->GetTeamId() == selfTeamId)
		{
			continue;
		}
		
		auto const squaredDistance = (avatar->GetActorLocation() - selfLocation).SizeSquared();
		
		if(closestDistance > squaredDistance)
		{
			closestDistance = squaredDistance;
			closestEnemyAvatar = avatar;
		}
	}

	if(closestEnemyAvatar == nullptr)
	{
		return TaskFailed(blackboardComponent);
	}

	blackboardComponent->SetValueAsObject(_enemyAvatarKeyName, closestEnemyAvatar);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UFindEnemyBTTaskNode::TaskFailed(UBlackboardComponent* blackboardComponent)
{
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);
	
	blackboardComponent->SetValueAsObject(_enemyAvatarKeyName, nullptr);
	return EBTNodeResult::Failed;
}
