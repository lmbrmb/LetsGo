#pragma once

#include "WeaponId.h"
#include "LetsGo/Data/PlayerId.h"

struct Damage
{
public:
	Damage();

	// ReSharper disable once CppNonExplicitConvertingConstructor
	Damage(const float amount);
	
	Damage(
		const PlayerId& instigatorPlayerId,
		const WeaponId& instigatorWeaponId,
		const float amount
	);

	~Damage() = default;
	
	const PlayerId& GetInstigatorPlayerId() const;

	const WeaponId& GetInstigatorWeaponId() const;
	
	float GetAmount() const;

private:
	PlayerId _instigatorPlayerId;

	WeaponId _instigatorWeaponId;
	
	float _amount;
};
