#include "Gun.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	_clipCurrent = _clipMax;
}

void AGun::Tick(float DeltaSeconds)
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
	}
}

void AGun::SetState(GunState state)
{
	_state = state;
}

void AGun::ProcessIdleState()
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

void AGun::ProcessReloadingState()
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

void AGun::ProcessShootingState()
{
	if (IsShooting())
	{
		return;
	}
	
	SetState(GunState::Idle);
}

void AGun::StartFire()
{
	TriggerFire(true);
}

void AGun::StopFire()
{
	TriggerFire(false);
}

void AGun::Reload()
{
	if (IsClipFull() || IsLoadingAmmoStarted())
	{
		return;
	}

	StartReload();
}

void AGun::SetAmmoProvider(AmmoProvider* ammoProvider)
{
	_ammoProvider = ammoProvider;
}

USceneComponent* AGun::GetFirePivot()
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

void AGun::StartShot()
{
	_shotStartTime = GetWorld()->TimeSeconds;
	ConsumeClip(_consumeAmmoPerShot);
	auto const firePivot = GetFirePivot();
	ShotPerformed.Broadcast(firePivot, AimProvider);
	SetState(GunState::Shooting);
}

bool AGun::IsShooting() const
{
	auto const isShooting = _shotStartTime > 0 && GetWorld()->TimeSeconds - _shotStartTime < _shotDuration;
	return isShooting;
}

void AGun::AddToClip(const int amount)
{
	_clipCurrent += amount;
}

void AGun::ConsumeClip(const int amount)
{
	_clipCurrent -= amount;
}

bool AGun::IsClipFull() const
{
	return _clipCurrent == _clipMax;
}

bool AGun::IsEnoughAmmoForShot() const
{
	auto const isEnoughAmmoForShot = _clipCurrent >= _consumeAmmoPerShot;
	return isEnoughAmmoForShot;
}

bool AGun::HasAmmoToLoad() const
{
	auto const hasAmmoToLoad = GetAmmoCount() >= _consumeAmmoPerShot;
	return hasAmmoToLoad;
}

void AGun::StartReload()
{
	_reloadStartTime = GetWorld()->TimeSeconds;
	BpReloadStarted();
	SetState(GunState::Reloading);
}

int AGun::GetAmmoCount() const
{
	return _ammoProvider == nullptr ? 0 : _ammoProvider->Get();
}

void AGun::ConsumeAmmo(const int amount) const
{
	if(_ammoProvider == nullptr)
	{
		return;
	}

	_ammoProvider->Remove(amount);
}

void AGun::LoadAmmo()
{
	_ammoLoadStartTime = GetWorld()->TimeSeconds;
	auto const ammoDelta = _clipMax - _clipCurrent;
	auto ammoToLoad = FMath::Min(ammoDelta, GetAmmoCount());
	ammoToLoad = FMath::Min(ammoToLoad, _ammoPerLoad);
	AddToClip(ammoToLoad);
	ConsumeAmmo(ammoToLoad);
	BpAmmoLoaded();
}

void AGun::FinishReload()
{
	_ammoLoadStartTime = UNDEFINED_TIME;
	_reloadStartTime = UNDEFINED_TIME;
	BpReloadFinished();
	SetState(GunState::Idle);
}

bool AGun::IsLoadAmmoDelayActive() const
{
	auto const isLoadAmmoDelayActive = GetWorld()->TimeSeconds - _reloadStartTime < _loadAmmoDelay;
	return isLoadAmmoDelayActive;
}

bool AGun::IsLoadingAmmo() const
{
	auto const isLoadingAmmo = GetWorld()->TimeSeconds - _ammoLoadStartTime < _ammoLoadDuration;
	return isLoadingAmmo;
}

bool AGun::IsLoadingAmmoStarted() const
{
	return _ammoLoadStartTime > 0;
}

void AGun::TriggerFire(bool isFireTriggered)
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
