#include "GunRecoilAnimationMapping.h"

#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

UGunRecoilAnimationMapping::UGunRecoilAnimationMapping()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGunRecoilAnimationMapping::Map()
{
	AssertNameIsAssigned(_animationId);

	auto const owner = GetOwner();
	
	TArray<UCurveAnimationComponent*> curveAnimationComponents;
	owner->GetComponents<UCurveAnimationComponent>(curveAnimationComponents);

	auto matchCount = 0;
	for (auto curveAnimationComponent : curveAnimationComponents)
	{
		if(curveAnimationComponent->GetAnimationId() == _animationId)
		{
			matchCount++;
			_curveAnimationComponent = curveAnimationComponent;
		}
	}

	AssertIsNotNull(_curveAnimationComponent);
	AssertIsEqual(matchCount, 1);

	auto const weaponManager = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManager);
	
	weaponManager->ShotPerformed.AddUObject(this, &UGunRecoilAnimationMapping::OnShotPerformed);
	weaponManager->WeaponEquipped.AddUObject(this, &UGunRecoilAnimationMapping::OnWeaponEquipped);

	_isInitialized = true;
}

void UGunRecoilAnimationMapping::OnShotPerformed(
	const PlayerId& playerId,
	const WeaponType& weaponType,
	const bool isAnyBulletDamaged
) const
{
	_curveAnimationComponent->SetState(FCurveAnimationState::ApplyCurve);
}

void UGunRecoilAnimationMapping::OnWeaponEquipped() const
{
	_curveAnimationComponent->SetState(FCurveAnimationState::MoveToOrigin);
}
