#pragma once

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponId.h"
#include "LetsGo/Data/WeaponType.h"

DECLARE_EVENT(IWeapon, EWeaponInitialized);

DECLARE_EVENT(IWeapon, ERequestReady);

///<summary>
/// [Interface] Weapon contract
///</summary>
class IWeapon
{
	// Not virtual - default implementation is ok
public:
	virtual ~IWeapon() = default;

	EWeaponInitialized WeaponInitialized;

	ERequestReady RequestReady;

	bool IsWeaponInitialized() const;

	void InitializeWeapon(const WeaponId& weaponId, const WeaponType& weaponType, const PlayerId& playerId);

	virtual bool IsRequestReady() const = 0;

	const WeaponId& GetWeaponId() const;

	const WeaponType& GetWeaponType() const;

	const PlayerId& GetPlayerId() const;

private:
	WeaponId _weaponId;

	WeaponType _weaponType;

	PlayerId _playerId;

	bool _isWeaponInitialized = false;
};
