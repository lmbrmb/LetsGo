#include "IWeapon.h"

void IWeapon::InitializeWeapon(const WeaponId& id, const PlayerId& playerId)
{
	_id = id;
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
	return _id;
}

const PlayerId& IWeapon::GetPlayerId() const
{
	return _playerId;
}
