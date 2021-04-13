#include "GunV1.h"

#include "LetsGo/Logs/DevLogger.h"

AGunV1::AGunV1()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunV1::OnShotPerformed(const bool isAnyBulletDamaged)
{
	ShotPerformed.Broadcast(this, isAnyBulletDamaged);
}

void AGunV1::OnBulletTraced(const bool isDamaged, const FHitResult& hitResult)
{
	BpOnBulletTraced(isDamaged, hitResult);
}

void AGunV1::BeginPlay()
{
	Super::BeginPlay();

	_clipCurrent = _clipMax;
	
	_stateProcessors.Add(GunState::Idle, [this]() { ProcessIdleState(); });
	_stateProcessors.Add(GunState::Shooting, [this]() { ProcessShootingState(); });
	_stateProcessors.Add(GunState::Reloading, [this]() { ProcessReloadingState(); });
	
	SetState(GunState::Idle);
}

void AGunV1::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (_stateProcessor)
	{
		_stateProcessor();
	}
}

void AGunV1::SetState(const GunState state)
{
	if (_state == state)
	{
		DevLogger::GetLoggingChannel()->LogValue("Same state transition. State:", (int)state, LogSeverity::Error);
		return;
	}
	
	_state = state;

	if (!_stateProcessors.Contains(_state))
	{
		DevLogger::GetLoggingChannel()->LogValue("Unhandled state", static_cast<int>(_state), LogSeverity::Error);
		return;
	}

	_stateProcessor = _stateProcessors[_state];

	if (_state == GunState::Idle)
	{
		RequestReady.Broadcast();
	}
}

void AGunV1::ProcessIdleState()
{
	if (!_isFireTriggered)
	{
		return;
	}

	if (IsEnoughAmmoForShot())
	{
		ConsumeAmmo(_consumeAmmoPerShot);
		StartShot();
		return;
	}

	if (HasAmmoToLoad())
	{
		StartReload();
		return;
	}

	StopFire();
	OutOfAmmo.Broadcast();
}

void AGunV1::ProcessReloadingState()
{
	if(IsLoadAmmoDelayActive())
	{
		return;
	}
	
	if (!IsLoadingAmmo())
	{
		if(IsClipFull() || !HasAmmoToLoad())
		{
			FinishReload();
		}
		else
		{
			LoadAmmo();
		}
	}
}

void AGunV1::ProcessShootingState()
{
	if (IsShooting())
	{
		return;
	}
	
	SetState(GunState::Idle);
}

void AGunV1::StartFire()
{
	TriggerFire(true);
}

void AGunV1::StopFire()
{
	TriggerFire(false);
}

void AGunV1::Reload()
{
	if (IsClipFull() || IsLoadingAmmoStarted())
	{
		return;
	}

	StartReload();
}

USceneComponent* AGunV1::GetFirePivot()
{
	int nextIndex = 0;
	switch (_firePivotMode)
	{
	case FFirePivotMode::Cycle:
		nextIndex = _firePivotIndex + 1;

		if (nextIndex >= _firePivots.Num())
		{
			nextIndex = 0;
		}
		break;
	case FFirePivotMode::AlwaysFirst:
	default:
		break;
	}
	_firePivotIndex = nextIndex;
	return _firePivots[_firePivotIndex];
}

void AGunV1::StartShot()
{
	_shotStartTime = GetWorld()->TimeSeconds;
	_clipCurrent -= _consumeAmmoPerShot;

	if(_firePivotMode == FFirePivotMode::AllSimultaneously)
	{
		for (auto firePivot : _firePivots)
		{
			ShotRequested.Broadcast(firePivot);
			BpOnShotRequest(firePivot);
		}
	}
	else
	{
		auto const firePivot = GetFirePivot();
		ShotRequested.Broadcast(firePivot);
		BpOnShotRequest(firePivot);
	}
	
	SetState(GunState::Shooting);
}

bool AGunV1::IsShooting() const
{
	auto const isShooting = _shotStartTime > 0 && GetWorld()->TimeSeconds - _shotStartTime < _shotDuration;
	return isShooting;
}

bool AGunV1::IsClipFull() const
{
	return _clipCurrent == _clipMax;
}

bool AGunV1::IsEnoughAmmoForShot() const
{
	auto const isEnoughAmmoForShot = _clipCurrent >= _consumeAmmoPerShot;
	return isEnoughAmmoForShot;
}

bool AGunV1::IsRequestReady() const
{
	return _state == GunState::Idle;
}

bool AGunV1::HasAmmoToLoad() const
{
	auto const hasAmmoToLoad = GetAmmoCount() >= _consumeAmmoPerShot;
	return hasAmmoToLoad;
}

void AGunV1::StartReload()
{
	_reloadStartTime = GetWorld()->TimeSeconds;
	BpOnReloadStarted();
	SetState(GunState::Reloading);
}

int AGunV1::GetAmmoCount() const
{
	return GetAmmoProvider() == nullptr ? 0 : GetAmmoProvider()->GetCurrent();
}

void AGunV1::ConsumeAmmo(const int amount) const
{
	if(GetAmmoProvider() == nullptr)
	{
		return;
	}

	GetAmmoProvider()->Remove(amount);
}

void AGunV1::LoadAmmo()
{
	_ammoLoadStartTime = GetWorld()->TimeSeconds;
	auto const ammoDelta = _clipMax - _clipCurrent;
	auto ammoToLoad = FMath::Min(ammoDelta, GetAmmoCount());
	ammoToLoad = FMath::Min(ammoToLoad, _ammoPerLoad);
	_clipCurrent += ammoToLoad;
	ConsumeAmmo(ammoToLoad);
	BpOnAmmoLoaded();
}

void AGunV1::FinishReload()
{
	_ammoLoadStartTime = UNDEFINED_TIME;
	_reloadStartTime = UNDEFINED_TIME;
	BpOnReloadFinished();
	SetState(GunState::Idle);
}

bool AGunV1::IsLoadAmmoDelayActive() const
{
	auto const isLoadAmmoDelayActive = GetWorld()->TimeSeconds - _reloadStartTime < _loadAmmoDelay;
	return isLoadAmmoDelayActive;
}

bool AGunV1::IsLoadingAmmo() const
{
	auto const isLoadingAmmo = GetWorld()->TimeSeconds - _ammoLoadStartTime < _ammoLoadDuration;
	return isLoadingAmmo;
}

bool AGunV1::IsLoadingAmmoStarted() const
{
	return _ammoLoadStartTime > 0;
}

void AGunV1::TriggerFire(bool const isFireTriggered)
{
	auto const prevIsFiringTriggered = _isFireTriggered;
	_isFireTriggered = isFireTriggered;
	if (prevIsFiringTriggered != _isFireTriggered)
	{
		if(_isFireTriggered)
		{
			BpOnFireStarted();
		}
		else
		{
			BpOnFireStopped();
		}
	}
}
