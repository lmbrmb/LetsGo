#pragma once

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"
#include "LetsGo/Data/WeaponType.h"

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

	void InitializeWeapon(const WeaponId& weaponId, const WeaponType& weaponType, const PlayerId& playerId);

	const WeaponId& GetWeaponId() const;

	const WeaponType& GetWeaponType() const;

	const PlayerId& GetPlayerId() const;

private:
	WeaponId _weaponId;

	WeaponType _weaponType;

	PlayerId _playerId;

	bool _isWeaponInitialized = false;
};
