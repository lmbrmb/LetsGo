#include "StopFireBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

EBTNodeResult::Type UStopFireBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);

	auto const selfActor = Cast<AActor>(selfActorObject);
	AssertIsNotNull(selfActor, EBTNodeResult::Failed);
	
	auto const weaponManager = selfActor->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManager, EBTNodeResult::Failed);
	
	weaponManager->StopFire();
	return EBTNodeResult::Succeeded;
}
