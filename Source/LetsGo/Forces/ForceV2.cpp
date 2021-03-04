#include "ForceV2.h"

#include "LetsGo/Utils/AssertUtils.h"

ForceV2::ForceV2(
	const FName id,
	const FVector direction,
	UCurveFloat* magnitudeCurve
	) :
	_id(id),
	_direction(direction),
	_magnitudeCurve(magnitudeCurve)
{
	AssertIsNotNull(magnitudeCurve);
}

FName ForceV2::GetId() const
{
	return _id;
}

FVector ForceV2::GetVector(const float deltaTime)
{
	if(!_magnitudeCurve)
	{
		return FVector::ZeroVector;
	}
	
	auto const magnitude = _magnitudeCurve->GetFloatValue(_lifeTime);
	auto const vector = _direction * magnitude;
	_lifeTime += deltaTime;
	return vector;
}
