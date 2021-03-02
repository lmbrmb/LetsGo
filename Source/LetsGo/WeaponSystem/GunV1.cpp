#include "GunV1.h"

AGunV1::AGunV1()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunV1::BeginPlay()
{
	Super::BeginPlay();

	_clipCurrent = _clipMax;
}

void AGunV1::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (_state)
	{
		case GunState::Idle:
			ProcessIdleState();
			break;
		case  GunState::Reloading:
			ProcessReloadingState();
			break;
		case GunState::Shooting:
			ProcessShootingState();
			break;
		default:
			DevLogger::GetLoggingChannel()->Log("Unhandled state", LogSeverity::Error);
			break;
	}
}

void AGunV1::SetState(GunState state)
{
	_state = state;
}

void AGunV1::ProcessIdleState()
{
	if(IsEnoughAmmoForShot())
	{
		// Shot requested
		if(_isFireTriggered)
		{
			StartShot();
		}
	}
	else
	{
		// Automatic reload
		if (HasAmmoToLoad())
		{
			StartReload();
		}
	}
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

void AGunV1::Init(AmmoProvider* ammoProvider, USceneComponent* aimProvider)
{
	_ammoProvider = ammoProvider;
	_aimProvider = aimProvider;
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

void AGunV1::StartShot()
{
	_shotStartTime = GetWorld()->TimeSeconds;
	_clipCurrent -= _consumeAmmoPerShot;
	auto const firePivot = GetFirePivot();
	ShotPerformed.Broadcast(firePivot, _aimProvider);
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

bool AGunV1::HasAmmoToLoad() const
{
	auto const hasAmmoToLoad = GetAmmoCount() >= _consumeAmmoPerShot;
	return hasAmmoToLoad;
}

void AGunV1::StartReload()
{
	_reloadStartTime = GetWorld()->TimeSeconds;
	BpReloadStarted();
	SetState(GunState::Reloading);
}

int AGunV1::GetAmmoCount() const
{
	return _ammoProvider == nullptr ? 0 : _ammoProvider->GetCurrent();
}

void AGunV1::ConsumeAmmo(const int amount) const
{
	if(_ammoProvider == nullptr)
	{
		return;
	}

	_ammoProvider->Remove(amount);
}

void AGunV1::LoadAmmo()
{
	_ammoLoadStartTime = GetWorld()->TimeSeconds;
	auto const ammoDelta = _clipMax - _clipCurrent;
	auto ammoToLoad = FMath::Min(ammoDelta, GetAmmoCount());
	ammoToLoad = FMath::Min(ammoToLoad, _ammoPerLoad);
	_clipCurrent += ammoToLoad;
	ConsumeAmmo(ammoToLoad);
	BpAmmoLoaded();
}

void AGunV1::FinishReload()
{
	_ammoLoadStartTime = UNDEFINED_TIME;
	_reloadStartTime = UNDEFINED_TIME;
	BpReloadFinished();
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
			BpFireStarted();
		}
		else
		{
			BpFireStopped();
		}
	}
}
