#include "AttackBTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LetsGo/AimProviders/DirectAimProvider.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/FVectorUtils.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

EBTNodeResult::Type UAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const blackboardComponent = OwnerComp.GetBlackboardComponent();
	AssertIsNotNull(blackboardComponent, EBTNodeResult::Failed);

	auto const selfActorObject = blackboardComponent->GetValueAsObject(_selfActorKeyName);
	AssertIsNotNull(selfActorObject, EBTNodeResult::Failed);
	
	auto const selfAvatar = Cast<AAvatar>(selfActorObject);
	AssertIsNotNull(selfAvatar, EBTNodeResult::Failed);

	auto const weaponManager = selfAvatar->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManager, EBTNodeResult::Failed);
	
	auto const enemyAvatarObject = blackboardComponent->GetValueAsObject(_enemyAvatarKeyName);

	if(!enemyAvatarObject)
	{
		weaponManager->StopFire();
		return EBTNodeResult::Failed;
	}
	
	auto const enemyAvatar = Cast<AAvatar>(enemyAvatarObject);
	AssertIsNotNull(selfAvatar, EBTNodeResult::Failed);

	auto const aimProvider = weaponManager->GetAimProvider();
	AssertIsNotNull(aimProvider, EBTNodeResult::Failed);

	auto const directAimProvider = dynamic_cast<DirectAimProvider*>(aimProvider);
	AssertIsNotNull(directAimProvider, EBTNodeResult::Failed);

	auto const enemyLocation = enemyAvatar->GetActorLocation();
	auto const enemyLocation2D = FVectorUtils::ToFVector2D(enemyLocation);

	auto const selfLocation = selfAvatar->GetActorLocation();
	auto const selfLocation2D = FVectorUtils::ToFVector2D(selfLocation);
	
	auto const locationDelta2D = enemyLocation2D - selfLocation2D;
	auto const distanceSquared2D = locationDelta2D.SizeSquared();

	auto const aimDirection2D = locationDelta2D.GetSafeNormal();
	auto const selfForward = selfAvatar->GetActorForwardVector();
	auto const selfForward2D = FVector2D(selfForward.X, selfForward.Y);
	auto const aimDot2D = FVector2D::DotProduct(selfForward2D, aimDirection2D);

	if(aimDot2D < _minAimDotStartFire)
	{
		weaponManager->StopFire();
		return EBTNodeResult::Succeeded;
	}
	
	auto const aimOffsetX = GetRandomOffset();
	auto const aimOffsetY = GetRandomOffset();
	auto const aimOffsetZ = GetRandomOffset();
	auto const distanceCoefficient = distanceSquared2D / _aimOffsetDistanceSquared;
	auto const dispersion = FVector(aimOffsetX, aimOffsetY, aimOffsetZ) * distanceCoefficient;
	
	directAimProvider->SetTargetAimLocation(enemyLocation + _avatarCenterOffset + dispersion);
	weaponManager->StartFire();
	return EBTNodeResult::Succeeded;
}

float UAttackBTTaskNode::GetRandomOffset() const
{
	return FMath::RandRange(-_aimOffsetRadius, _aimOffsetRadius);
}
