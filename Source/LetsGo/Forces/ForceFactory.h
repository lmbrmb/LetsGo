#pragma once

#include "IForce.h"

/// <summary>
/// Force factory
/// </summary>
class ForceFactory final
{
public:
	explicit ForceFactory() = default;
	
	IForce* Create(
		const FName id,
		const FVector direction,
		const float magnitude
	);

	IForce* Create(
		const FName id,
		const FVector direction,
		UCurveFloat* magnitudeCurve
	);

	IForce* Create(
		const FName id,
		const FVector direction,
		UCurveFloat* magnitudeCurve,
		const float curveMagnitudeMultiplier,
		const float curveTimeMultiplier
	);
};

Expose_TNameOf(ForceFactory)
