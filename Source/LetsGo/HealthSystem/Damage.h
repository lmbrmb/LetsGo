#pragma once

struct Damage
{
public:
	Damage();

	Damage(const float amount);
	
	Damage(
		const int32 instigatorId,
		const FName weaponId,
		const float amount
	);

	~Damage() = default;
	
	int32 GetInstigatorId() const;

	FName GetWeaponId() const;
	
	float GetAmount() const;

private:
	int32 _instigatorId;

	FName _weaponId;
	
	float _amount;
};
