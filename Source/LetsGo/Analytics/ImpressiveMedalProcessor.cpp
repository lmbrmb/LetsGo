#include "ImpressiveMedalProcessor.h"

const int ImpressiveMedalProcessor::REQUIRED_HIT_COUNT = 2;

const FName ImpressiveMedalProcessor::RAILGUN_ID = "Railgun";

bool ImpressiveMedalProcessor::ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal)
{
	if(damageEvent.InstigatorWeaponId != RAILGUN_ID)
	{
		return false;
	}

	auto const damagedPlayerIsDead = damageEvent.DamagedPlayerHealth <= 0;
	if(damagedPlayerIsDead)
	{
		//Reset fragged player hit count
		auto const fraggedPlayerId = damageEvent.DamagedPlayerId;
		if (_playerRailgunHitCount.Contains(fraggedPlayerId))
		{
			_playerRailgunHitCount[fraggedPlayerId] = 0;
		}
		else
		{
			_playerRailgunHitCount.Add(fraggedPlayerId, 0);
		}
	}
	
	auto const instigatorPlayerId = damageEvent.InstigatorPlayerId;

	if(!_playerRailgunHitCount.Contains(instigatorPlayerId))
	{
		// Optimization valid for REQUIRED_HIT_COUNT > 1
		_playerRailgunHitCount.Add(instigatorPlayerId, 1);
		return false;
	}

	_playerRailgunHitCount[instigatorPlayerId]++;
	auto const hitCount = _playerRailgunHitCount[instigatorPlayerId];
	if(hitCount >= REQUIRED_HIT_COUNT)
	{
		outMedal.PlayerId = damageEvent.InstigatorPlayerId;
		outMedal.MedalType = FMedalType::Impressive;
		_playerRailgunHitCount[instigatorPlayerId] = 0;
		return true;
	}

	return false;
}
