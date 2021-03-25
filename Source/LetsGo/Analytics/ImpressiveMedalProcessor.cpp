#include "ImpressiveMedalProcessor.h"

ImpressiveMedalProcessor::ImpressiveMedalProcessor(
	const int requiredHitCount,
	const WeaponType& weaponType
) :
	_requiredHitCount(requiredHitCount),
	_weaponType(weaponType)
{
}

bool ImpressiveMedalProcessor::ProcessDamageEvent(const DamageEvent& damageEvent, Medal& outMedal)
{
	auto const damagedPlayerIsDead = damageEvent.GetDamagedPlayerHealth() <= 0;
	if(damagedPlayerIsDead)
	{
		//Reset fragged player hit count
		auto const fraggedPlayerIdValue = damageEvent.GetDamagedPlayerId().GetId();
		if (_playerRailgunHitCount.Contains(fraggedPlayerIdValue))
		{
			_playerRailgunHitCount[fraggedPlayerIdValue] = 0;
		}
		else
		{
			_playerRailgunHitCount.Add(fraggedPlayerIdValue, 0);
		}
	}

	return false;
}

bool ImpressiveMedalProcessor::ProcessHitEvent(const HitEvent& hitEvent, Medal& outMedal)
{
	if (hitEvent.GetInstigatorWeaponType() != _weaponType)
	{
		return false;
	}

	//TODO: check self harm
	
	auto const instigatorPlayerIdValue = hitEvent.GetInstigatorPlayerId().GetId();

	if (!_playerRailgunHitCount.Contains(instigatorPlayerIdValue))
	{
		_playerRailgunHitCount.Add(instigatorPlayerIdValue, 0);
	}

	if(!hitEvent.GetIsHittedPlayer())
	{
		_playerRailgunHitCount[instigatorPlayerIdValue] = 0;
		return false;
	}
	
	_playerRailgunHitCount[instigatorPlayerIdValue]++;
	
	auto const hitCount = _playerRailgunHitCount[instigatorPlayerIdValue];
	if (hitCount >= _requiredHitCount)
	{
		const Medal impressiveMedal(hitEvent.GetInstigatorPlayerId(), FMedalType::Impressive);
		outMedal = impressiveMedal;
		_playerRailgunHitCount[instigatorPlayerIdValue] = 0;
		return true;
	}
	return false;
}
