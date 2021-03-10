#pragma once

struct Damage
{
public:
	explicit Damage(const int32 instigatorId, const float amount);

	~Damage() = default;
	
	int32 GetInstigatorId() const;

	float GetAmount() const;

private:
	int32 _instigatorId;
	
	float _amount;
};
