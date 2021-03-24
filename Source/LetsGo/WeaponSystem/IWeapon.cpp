#include "IWeapon.h"

void IWeapon::InitializeWeapon(const WeaponId& weaponId, const PlayerId& playerId)
{
	_weaponId = weaponId;
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

const PlayerId& IWeapon::GetPlayerId() const
{
	return _playerId;
}
