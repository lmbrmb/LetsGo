#pragma once

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"

class IWeapon;

DECLARE_EVENT_OneParam(IWeapon, EWeaponInitialized, IWeapon*);

///<summary>
/// [Interface] Weapon contract
///</summary>
class IWeapon
{
	// Not virtual - default implementation is ok
public:
	EWeaponInitialized WeaponInitialized;
	
	bool IsWeaponInitialized() const;

	void InitializeWeapon(const WeaponId& id, const PlayerId& playerId);

	const WeaponId& GetWeaponId() const;

	const PlayerId& GetPlayerId() const;
	
private:
	WeaponId _id;

	PlayerId _playerId;
	
	bool _isWeaponInitialized = false;
};
