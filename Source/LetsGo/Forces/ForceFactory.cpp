#pragma once

#include "ForceFactory.h"
#include "ForceV1.h"
#include "ForceV2.h"
#include "ForceV3.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IForce* ForceFactory::Create(
	const FName& id,
	const FVector& direction,
	const float magnitude
)
{
	return new ForceV1(id, direction, magnitude);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
IForce* ForceFactory::Create(
	const FName& id,
	const FVector& direction,
	UCurveFloat* magnitudeCurve
)
{
	return new ForceV2(id, direction, magnitudeCurve);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
IForce* ForceFactory::Create(
	const FName& id,
	const FVector& direction,
	UCurveFloat* magnitudeCurve,
	const float curveMagnitudeMultiplier,
	const float curveTimeMultiplier
)
{
	return new ForceV3(id, direction, magnitudeCurve , curveMagnitudeMultiplier, curveTimeMultiplier);
}