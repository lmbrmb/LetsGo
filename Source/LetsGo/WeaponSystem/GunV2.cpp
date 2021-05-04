#include "GunV2.h"

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

AGunV2::AGunV2()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunV2::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(_stateProcessor)
	{
		_stateProcessor();
	}
}

void AGunV2::BeginPlay()
{
	Super::BeginPlay();
	
	_stateProcessors.Add(GunState::Idle, [this]() { ProcessIdleState(); });
	_stateProcessors.Add(GunState::Shooting, [this]() { ProcessShootingState(); });
	
	SetState(GunState::Idle);
}

void AGunV2::SetFirePivot(USceneComponent* firePivot)
{
	_firePivot = firePivot;
}

FVector AGunV2::GetTargetAimLocation() const
{
	auto const aimProvider = GetAimProvider();
	return aimProvider ? aimProvider->GetTargetAimLocation() : FVector::ZeroVector;
}

void AGunV2::SetState(const GunState state)
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

void AGunV2::ProcessIdleState()
{
	if (!_isFireTriggered)
	{
		return;
	}

	if(!IsEnoughAmmoForShot())
	{
		StopFire();
		OutOfAmmo.Broadcast();
		return;
	}

	ConsumeAmmo(_consumeAmmoPerShot);
	StartShot();
}

void AGunV2::ProcessShootingState()
{
	if (IsShooting())
	{
		return;
	}

	SetState(GunState::Idle);
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

void AGunV2::OnShotPerformed(const bool isAnyBulletDamaged)
{
	ShotPerformed.Broadcast(this, isAnyBulletDamaged);
}

void AGunV2::OnBulletTraced(const bool isDamaged, const FHitResult& hitResult)
{
	BpOnBulletTraced(isDamaged, hitResult);
}

void AGunV2::StartShot()
{
	AssertIsNotNull(_firePivot);

	_shotStartTime = GetWorld()->TimeSeconds;
	ShotRequested.Broadcast(_firePivot);
	BpOnShotRequest(_firePivot);
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

bool AGunV2::IsRequestReady() const
{
	return _state == GunState::Idle;
}

int AGunV2::GetAmmoCount() const
{
	return GetAmmoProvider() == nullptr ? 0 : GetAmmoProvider()->GetCurrent();
}

void AGunV2::ConsumeAmmo(const int amount) const
{
	if (GetAmmoProvider() == nullptr)
	{
		return;
	}

	GetAmmoProvider()->Remove(amount);
}

void AGunV2::TriggerFire(const bool isFireTriggered)
{
	auto const prevIsFiringTriggered = _isFireTriggered;
	_isFireTriggered = isFireTriggered;
	if (prevIsFiringTriggered != _isFireTriggered)
	{
		if (_isFireTriggered)
		{
			BpOnFireStarted();
		}
		else
		{
			BpOnFireStopped();
		}
	}
}
