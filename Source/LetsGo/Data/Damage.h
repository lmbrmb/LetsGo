#pragma once

#include "WeaponType.h"
#include "LetsGo/Data/PlayerId.h"

struct Damage
{
public:
	Damage();

	// ReSharper disable once CppNonExplicitConvertingConstructor
	Damage(const float amount);
	
	Damage(
		const PlayerId& instigatorPlayerId,
		const WeaponType& instigatorWeaponType,
		const float amount
	);

	~Damage() = default;
	
	const PlayerId& GetInstigatorPlayerId() const;

	const WeaponType& GetInstigatorWeaponType() const;
	
	float GetAmount() const;

private:
	PlayerId _instigatorPlayerId;

	WeaponType _instigatorWeaponType;
	
	float _amount;
};
