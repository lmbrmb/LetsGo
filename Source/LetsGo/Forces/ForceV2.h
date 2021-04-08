#pragma once

#include "IForce.h"

/// <summary>
/// Force variation 2: magnitude is set via curve
/// </summary>
class ForceV2 final : public IForce
{
public:
	ForceV2(
		const FName id,
		const FVector direction,
		UCurveFloat* magnitudeCurve
	);

	virtual FName GetId() const override;

	virtual FVector GetVector(const float deltaTime) override;

private:
	FName _id;

	FVector _direction;

	UCurveFloat* _magnitudeCurve;

	float _lifeTime = 0;
};
