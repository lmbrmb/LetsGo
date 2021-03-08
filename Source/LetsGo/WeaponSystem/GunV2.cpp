#include "GunV2.h"

AGunV2::AGunV2()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunV2::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (_state)
	{
		case GunState::Idle:
			ProcessIdleState();
			break;
		case GunState::Shooting:
			ProcessShootingState();
			break;
		case  GunState::Reloading:
			DevLogger::GetLoggingChannel()->Log("Reloading state is not supported.", LogSeverity::Error);
			break;
		default:
			DevLogger::GetLoggingChannel()->Log("Unhandled state", LogSeverity::Error);
			break;
	}
}

void AGunV2::SetState(const GunState state)
{
	_state = state;
}

void AGunV2::ProcessIdleState()
{
	if (IsEnoughAmmoForShot())
	{
		// Shot requested
		if (_isFireTriggered)
		{
			StartShot();
		}
	}
}

void AGunV2::ProcessShootingState()
{
	if (IsShooting())
	{
		return;
	}

	SetState(GunState::Idle);
}

void AGunV2::Init(const FGuid instigatorId, AmmoProvider* ammoProvider, USceneComponent* aimProvider)
{
	_instigatorId = instigatorId;
	_ammoProvider = ammoProvider;
	_aimProvider = aimProvider;
}

void AGunV2::StartFire()
{
	TriggerFire(true);
}

void AGunV2::StopFire()
{
	TriggerFire(false);
}

void AGunV2::Reload()
{
	// Do nothing
}

USceneComponent* AGunV2::GetFirePivot()
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

void AGunV2::StartShot()
{
	_shotStartTime = GetWorld()->TimeSeconds;
	auto const firePivot = GetFirePivot();
	ShotPerformed.Broadcast(_instigatorId, firePivot, _aimProvider);
	SetState(GunState::Shooting);
}

bool AGunV2::IsShooting() const
{
	auto const isShooting = _shotStartTime > 0 && GetWorld()->TimeSeconds - _shotStartTime < _shotDuration;
	return isShooting;
}

bool AGunV2::IsEnoughAmmoForShot() const
{
	auto const isEnoughAmmoForShot = GetAmmoCount() >= _consumeAmmoPerShot;
	return isEnoughAmmoForShot;
}

int AGunV2::GetAmmoCount() const
{
	return _ammoProvider == nullptr ? 0 : _ammoProvider->GetCurrent();
}

void AGunV2::ConsumeAmmo(const int amount) const
{
	if (_ammoProvider == nullptr)
	{
		return;
	}

	_ammoProvider->Remove(amount);
}

void AGunV2::TriggerFire(const bool isFireTriggered)
{
	auto const prevIsFiringTriggered = _isFireTriggered;
	_isFireTriggered = isFireTriggered;
	if (prevIsFiringTriggered != _isFireTriggered)
	{
		if (_isFireTriggered)
		{
			BpFireStarted();
		}
		else
		{
			BpFireStopped();
		}
	}
}
