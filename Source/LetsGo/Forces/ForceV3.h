#pragma once

#include "IForce.h"

/// <summary>
/// Force variation 3: magnitude is set via curve and curve modifiers
/// </summary>
class ForceV3 final : public IForce
{
public:
	ForceV3(
		const FName& id,
		const FVector& direction,
		UCurveFloat* magnitudeCurve,
		const float curveMagnitudeMultiplier,
		const float curveTimeMultiplier
	);
	
	virtual const FName& GetId() const override;

	virtual FVector GetVector(const float deltaTime) override;

private:
	FName _id;

	FVector _direction;

	float _curveMagnitudeMultiplier;

	float _curveTimeMultiplier;

	UCurveFloat* _magnitudeCurve;

	float _lifeTime = 0;
};
