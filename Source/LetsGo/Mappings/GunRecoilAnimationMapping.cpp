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
	AssertIsTrue(_curveUp || _curveRight || _curveForward);
	AssertNameIsAssigned(_animationId);

	auto const owner = GetOwner();
	
	TArray<UCurveAnimationBlenderComponent*> curveAnimationBlenderComponents;
	owner->GetComponents<UCurveAnimationBlenderComponent>(curveAnimationBlenderComponents);

	auto matchCount = 0;
	for (auto curveAnimationBlenderComponent : curveAnimationBlenderComponents)
	{
		if (curveAnimationBlenderComponent->GetBlenderId() == _blenderId)
		{
			matchCount++;
			_curveAnimationBlenderComponent = curveAnimationBlenderComponent;
		}
	}

	AssertIsNotNull(_curveAnimationBlenderComponent);
	AssertIsEqual(matchCount, 1);

	auto const weaponManager = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManager);

	SetupAnimation();

	weaponManager->ShotRequested.AddUObject(this, &UGunRecoilAnimationMapping::OnShotRequested);

	_isInitialized = true;
}

void UGunRecoilAnimationMapping::SetupAnimation() const
{
	_curveAnimationBlenderComponent->AddAnimation(
		_animationId,
		_animationLoop,
		_isAnimationEnabled,
		_curveMagnitudeMultiplier,
		_curveTimeMultiplier
	);

	if (_curveUp)
	{
		_curveAnimationBlenderComponent->AddCurve(_animationId, _curveUp, FVector::UpVector);
	}

	if (_curveRight)
	{
		_curveAnimationBlenderComponent->AddCurve(_animationId, _curveRight, FVector::RightVector);
	}

	if (_curveForward)
	{
		_curveAnimationBlenderComponent->AddCurve(_animationId, _curveForward, FVector::ForwardVector);
	}
}

void UGunRecoilAnimationMapping::OnShotRequested() const
{
	_curveAnimationBlenderComponent->EnableAnimation(_animationId, true);
	_curveAnimationBlenderComponent->ResetAnimationTime(_animationId);
}
