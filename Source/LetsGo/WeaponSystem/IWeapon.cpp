#include "IWeapon.h"

void IWeapon::InitializeWeapon(const WeaponId& weaponId, const WeaponType& weaponType, const PlayerId& playerId)
{
	_weaponId = weaponId;
	_weaponType = weaponType;
	_playerId = playerId;
	_isWeaponInitialized = true;
	WeaponInitialized.Broadcast(this);
}

bool IWeapon::IsWeaponInitialized() const
{
	return _isWeaponInitialized;
}

const WeaponId& IWeapon::GetWeaponId() const
{
	return _weaponId;
}

const WeaponType& IWeapon::GetWeaponType() const
{
	return _weaponType;
}

const PlayerId& IWeapon::GetPlayerId() const
{
	return _playerId;
}
