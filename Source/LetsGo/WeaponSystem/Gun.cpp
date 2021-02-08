#include "Gun.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGun::Reload()
{
	StartReloading();
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	_ammoCount = _initialAmmoCount;
	FinishReloading();
}

void AGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsReloadingRequested())
	{
		if(!IsReloading())
		{
			FinishReloading();
		}
	}
}

void AGun::Fire()
{
	if(IsReloading())
	{
		return;
	}

	if(IsEnoughAmmoForShot())
	{
		ConsumeAmmo();
		auto const pivot = GetPivot();
		OnFire(pivot->GetComponentTransform());
	}
	
	if (!IsEnoughAmmoForShot() && CanReload())
	{
		Reload();
	}
}

USceneComponent* AGun::GetPivot()
{
	int nextIndex = 0;
	switch (_firePivotMode)
	{
	case FirePivotMode::Cycle:
		nextIndex = _firePivotIndex + 1;

		if (nextIndex >= _firePivots.Num())
		{
			nextIndex = 0;
		}
		break;
	case FirePivotMode::AlwaysFirst:
	default:
		break;
	}
	_firePivotIndex = nextIndex;
	return _firePivots[_firePivotIndex];
}

void AGun::ConsumeAmmo()
{
	_clipCurrent = _clipCurrent - _consumeAmmoPerShot;
}

bool AGun::IsEnoughAmmoForShot()
{
	return _clipCurrent >= _consumeAmmoPerShot;
}

bool AGun::CanReload() const
{
	return _ammoCount >= _consumeAmmoPerShot;
}

void AGun::StartReloading()
{
	if(IsReloadingRequested())
		return;

	OnReloadingStart();
	_startReloadTime = GetWorld()->TimeSeconds;
}

void AGun::FinishReloading()
{
	auto const ammoDelta = _clipMax - _clipCurrent;
	auto const ammoToLoad = FMath::Min(ammoDelta, _ammoCount);
	_clipCurrent += ammoDelta;
	_ammoCount -= ammoDelta;
	_startReloadTime = UNDEFINED_RELOADING_TIME;
}

bool AGun::IsReloading() const
{
	if (!IsReloadingRequested())
		return false;
	
	auto const isReloading = GetWorld()->TimeSeconds - _startReloadTime < _reloadTime;
	return isReloading;
}

bool AGun::IsReloadingRequested() const
{
	return _startReloadTime > 0;
}
