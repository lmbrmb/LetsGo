#include "Weapon.h"

void IWeapon::InitializeWeapon(const FName& id, const int32 playerId)
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

int32 IWeapon::GetPlayerId() const
{
	return _playerId;
}
