#include "Force.h"

Force::Force(FName id, FVector direction, float magnitude) :
	_id(id),
	_duration(0),
	_isInfinite(true)
{
	_vector = direction * magnitude;
}

Force::Force(FName id, float duration, FVector direction, float magnitude) :
	_id(id),
	_duration(duration),
	_isInfinite(false)
{
	_vector = direction * magnitude;
}

Force* Force::CreateInfiniteForce(FName id, FVector direction, float magnitude)
{
	return new Force(id, direction, magnitude);
}

Force* Force::CreateFiniteForce(FName id, float duration, FVector direction, float magnitude)
{
	return new Force(id, duration, direction, magnitude);
}

FName Force::GetId() const
{
	return _id;
}

bool Force::IsActive() const
{
	return _isInfinite || _duration > 0;
}

bool Force::IsInfinite() const
{
	return _isInfinite;
}

FVector Force::Take(float timeAmount)
{
	float durationDelta;

	if(_isInfinite)
	{
		durationDelta = timeAmount;
	}
	else
	{
		durationDelta = FMath::Min(timeAmount, _duration);
		_duration -= durationDelta;
	}
	
	return _vector * durationDelta;
}
