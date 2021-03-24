#include "DirectAimProvider.h"

#include "LetsGo/Logs/DevLogger.h"

DirectAimProvider::DirectAimProvider() :
_aimLocation(FVector::ZeroVector)
{
}

FVector DirectAimProvider::GetTargetAimLocation()
{
	return _aimLocation;
}

void DirectAimProvider::SetTargetAimLocation(const FVector aimLocation)
{
	_aimLocation = aimLocation;
}
