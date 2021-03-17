#include "IWeapon.h"

void IWeapon::InitializeWeapon(const FName& id, const PlayerId& playerId)
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

FName IWeapon::GetId() const
{
	return _id;
}

const PlayerId& IWeapon::GetPlayerId() const
{
	return _playerId;
}
