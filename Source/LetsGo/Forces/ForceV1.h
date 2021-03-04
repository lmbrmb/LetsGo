#pragma once

#include "IForce.h"

/// <summary>
/// Force variation 1: magnitude is set via constant scalar value
/// </summary>
class ForceV1 final : public IForce
{
public:
	explicit ForceV1(
		const FName id,
		const FVector direction,
		const float magnitude
	);
	
	virtual FName GetId() const override;
	
	virtual FVector GetVector(const float deltaTime) override;

private:
	FName _id;

	FVector _vector;
};
