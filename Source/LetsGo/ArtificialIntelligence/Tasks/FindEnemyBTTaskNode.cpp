#include "FindEnemyBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"

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

	AAvatar* enemyAvatar = nullptr;
	
	if(_lockOnSingleEnemy)
	{
		auto const enemyAvatarObject = blackboardComponent->GetValueAsObject(_enemyAvatarKeyName);
		enemyAvatar = Cast<AAvatar>(enemyAvatarObject);
		if (enemyAvatar)
		{
			auto const healthComponent = enemyAvatar->FindComponentByClass<UHealthComponent>();
			AssertIsNotNull(healthComponent, EBTNodeResult::Failed);

			if (healthComponent->IsAlive())
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	float closestDistance = MAX_int32;
	auto const selfLocation = selfAvatar->GetActorLocation();
	
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

		auto const healthComponent = avatar->FindComponentByClass<UHealthComponent>();
		AssertIsNotNull(healthComponent, EBTNodeResult::Failed);
		
		if (healthComponent->IsDead())
		{
			continue;
		}
		
		if(closestDistance > squaredDistance)
		{
			closestDistance = squaredDistance;
			enemyAvatar = avatar;
		}
	}

	if(enemyAvatar == nullptr)
	{
		blackboardComponent->SetValueAsObject(_enemyAvatarKeyName, nullptr);
		return EBTNodeResult::Succeeded;
	}

	blackboardComponent->SetValueAsObject(_enemyAvatarKeyName, enemyAvatar);

	auto const enemyLocation = enemyAvatar->GetActorLocation();

	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(selfAvatar);
	auto const isHitted = GetWorld()->LineTraceSingleByChannel(
		_hitResult,
		selfLocation + _avatarRayCastLocationOffset,
		enemyLocation + _avatarRayCastLocationOffset,
		_avatarRayCastCollisionChannel,
		collisionQueryParams
	);
	auto const isEnemyInLineOfSight = isHitted && enemyAvatar == _hitResult.GetActor();
	blackboardComponent->SetValueAsBool(_isEnemyInLineOfSightKeyName, isEnemyInLineOfSight);
	
	return EBTNodeResult::Succeeded;
}
