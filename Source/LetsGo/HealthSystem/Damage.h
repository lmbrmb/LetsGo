#pragma once

struct Damage
{
public:
	Damage(const FGuid instigatorId, const float amount);
	
	FGuid GetInstigatorId() const;

	float GetAmount() const;

private:
	const FGuid _instigatorId;
	
	const float _amount;
};
