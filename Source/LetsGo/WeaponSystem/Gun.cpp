#include "Gun.h"
#include "LetsGo/Logs/DevLogger.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGun::Reload()
{
	if(IsClipFull() || IsReloadStarted())
	{
		return;
	}
	
	StartReload();
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	_ammoCount = _initialAmmoCount;
	FinishReload();
}

void AGun::Tick(float DeltaSeconds)
{
	// TODO: state machine
	
	Super::Tick(DeltaSeconds);

	if (IsReloadStarted())
	{
		if (!IsReloading())
		{
			FinishReload();
		}
		return;
	}
	
	if (!IsEnoughAmmoForShot() && HasAmmoToReload())
	{
		Reload();
		return;
	}
	
	if (_isFireTriggered && !IsFiring() && IsEnoughAmmoForShot())
	{
		_startFireTime = GetWorld()->TimeSeconds;

		ConsumeAmmo();
		auto const pivot = GetPivot();
		OnShot(pivot->GetComponentTransform());
	}
}

void AGun::StartFire()
{
	TriggerFire(true);
}

void AGun::StopFire()
{
	TriggerFire(false);
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

bool AGun::IsFiring() const
{
	auto const isFiring = _startFireTime > 0 && GetWorld()->TimeSeconds - _startFireTime < _fireDuration;
	return isFiring;
}

void AGun::ConsumeAmmo()
{
	_clipCurrent = _clipCurrent - _consumeAmmoPerShot;
}

bool AGun::IsClipFull() const
{
	return _clipCurrent == _clipMax;
}

bool AGun::IsEnoughAmmoForShot()
{
	auto const isEnoughAmmoForShot = _clipCurrent >= _consumeAmmoPerShot;
	return isEnoughAmmoForShot;
}

bool AGun::HasAmmoToReload() const
{
	return _ammoCount >= _consumeAmmoPerShot;
}

void AGun::StartReload()
{
	OnReloadStarted();
	_startReloadTime = GetWorld()->TimeSeconds;
}

void AGun::FinishReload()
{
	auto const ammoDelta = _clipMax - _clipCurrent;
	auto const ammoToLoad = FMath::Min(ammoDelta, _ammoCount);
	_clipCurrent += ammoDelta;
	_ammoCount -= ammoDelta;
	_startReloadTime = UNDEFINED_TIME;
}

bool AGun::IsReloading() const
{
	auto const isReloading = GetWorld()->TimeSeconds - _startReloadTime < _reloadDuration;
	
	return isReloading;
}

bool AGun::IsReloadStarted() const
{
	return _startReloadTime > 0;
}

void AGun::TriggerFire(bool isFireTriggered)
{
	auto const prevIsFiringTriggered = _isFireTriggered;
	_isFireTriggered = isFireTriggered;
	if (prevIsFiringTriggered != _isFireTriggered)
	{
		if(_isFireTriggered)
		{
			OnFireStarted();
		}
		else
		{
			OnFireStopped();
		}
	}
}
