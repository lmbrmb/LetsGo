#pragma once

#include "LetsGo/Data/PlayerId.h"

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

	void InitializeWeapon(const FName& id, const PlayerId& playerId);

	FName GetId() const;

	const PlayerId& GetPlayerId() const;
	
private:
	FName _id;

	PlayerId _playerId;
	
	bool _isWeaponInitialized = false;
};
