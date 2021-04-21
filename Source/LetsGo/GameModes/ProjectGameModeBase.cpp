#include "ProjectGameModeBase.h"

void AProjectGameModeBase::RegisterUObject(UObject* uObject)
{
	AssertContainerDoesNotContainElement(_uObjectRegistry, uObject);
	_uObjectRegistry.Add(uObject);
}

void AProjectGameModeBase::UnRegisterUObject(UObject* uObject)
{
	AssertContainerContainsElement(_uObjectRegistry, uObject);
	_uObjectRegistry.Remove(uObject);
}
