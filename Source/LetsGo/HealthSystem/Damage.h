#pragma once

struct Damage
{
public:
	Damage(const FGuid instigatorId, const float amount);

	~Damage() = default;
	
	FGuid GetInstigatorId() const;

	float GetAmount() const;

private:
	FGuid _instigatorId;
	
	float _amount;
};
