#include "ForceV3.h"

#include "LetsGo/Utils/AssertUtils.h"

ForceV3::ForceV3(
	const FName id,
	const FVector direction,
	UCurveFloat* magnitudeCurve,
	const float curveMagnitudeMultiplier,
	const float curveTimeMultiplier
	) :
	_id(id),
	_direction(direction),
	_curveMagnitudeMultiplier(curveMagnitudeMultiplier),
	_curveTimeMultiplier(curveTimeMultiplier),
	_magnitudeCurve(magnitudeCurve)
{
	AssertIsNotNull(magnitudeCurve);
}

FName ForceV3::GetId() const
{
	return _id;
}

FVector ForceV3::GetVector(const float deltaTime)
{
	if(!_magnitudeCurve)
	{
		return FVector::ZeroVector;
	}
	
	auto const curveTime = _lifeTime * _curveTimeMultiplier;
	auto const curveValue = _magnitudeCurve->GetFloatValue(curveTime);
	auto const magnitude = curveValue * _curveMagnitudeMultiplier;
	auto const vector = _direction * magnitude;
	_lifeTime += deltaTime;
	return vector;
}
