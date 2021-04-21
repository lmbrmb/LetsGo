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
		const FHitResult& hitResult,
		const float amount
	);

	~Damage() = default;

	const PlayerId& GetInstigatorPlayerId() const;

	const WeaponType& GetInstigatorWeaponType() const;

	const FHitResult& GetHitResult() const;

	float GetAmount() const;

	bool GetIsHitted() const;

private:
	PlayerId _instigatorPlayerId;

	WeaponType _instigatorWeaponType;

	FHitResult _hitResult;

	float _amount;

	bool _isHitted;
};
