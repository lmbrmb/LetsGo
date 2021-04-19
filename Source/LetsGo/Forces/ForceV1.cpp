#include "ForceV1.h"

#include "LetsGo/Logs/DevLogger.h"

ForceV1::ForceV1(
	const FName& id,
	const FVector& direction,
	const float magnitude
	) :
	_id(id)
{
	_vector = direction * magnitude;
}

const FName& ForceV1::GetId() const
{
	return _id;
}

FVector ForceV1::GetVector(const float deltaTime)
{
	return _vector;
}
