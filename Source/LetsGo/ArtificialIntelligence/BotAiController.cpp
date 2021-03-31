#include "BotAiController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

void ABotAiController::BeginPlay()
{
	Super::BeginPlay();
	
	AssertIsNotNull(_behaviorTree);
}

void ABotAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	auto const avatar = Cast<AAvatar>(InPawn);
	AssertIsNotNull(avatar);
	
	AssertIsNotNull(_behaviorTree);

	auto const blackboardData = _behaviorTree->GetBlackboardAsset();
	AssertIsNotNull(blackboardData);
	
	RunBehaviorTree(_behaviorTree);

	//Blackboard is created in RunBehaviorTree()
	//Blackboard is initialized by _behaviorTree->GetBlackboardAsset()
	
	AssertIsNotNull(Blackboard);

	//UBehaviorTreeComponent is created in RunBehaviorTree()
	auto const behaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	AssertIsNotNull(behaviorTreeComponent);
}
