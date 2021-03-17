#pragma once

#include "LetsGo/Data/PlayerId.h"

struct Damage
{
public:
	Damage();

	// ReSharper disable once CppNonExplicitConvertingConstructor
	Damage(const float amount);
	
	Damage(
		const PlayerId& instigatorId,
		const FName& weaponId,
		const float amount
	);

	~Damage() = default;
	
	const PlayerId& GetInstigatorId() const;

	const FName& GetWeaponId() const;
	
	float GetAmount() const;

private:
	PlayerId _instigatorId;

	FName _weaponId;
	
	float _amount;
};
