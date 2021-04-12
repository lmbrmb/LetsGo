#include "ProjectGameModeBase.h"

void AProjectGameModeBase::RegisterUObject(UObject* uObject)
{
	AssertIsFalse(_uObjectRegistry.Contains(uObject));
	_uObjectRegistry.Add(uObject);
}

void AProjectGameModeBase::UnRegisterUObject(UObject* uObject)
{
	AssertIsTrue(_uObjectRegistry.Contains(uObject));
	_uObjectRegistry.Remove(uObject);
}
