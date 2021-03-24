#include "AimProviderComponent.h"

#include "LetsGo/Utils/AssertUtils.h"

UAimProviderComponent::UAimProviderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UAimProviderComponent::~UAimProviderComponent()
{
	if(_aimProvider)
	{
		delete _aimProvider;
	}
}

IAimProvider* UAimProviderComponent::GetAimProvider() const
{
	return _aimProvider;
}

void UAimProviderComponent::SetAimProvider(IAimProvider* aimProvider)
{
	if(_aimProvider)
	{
		delete _aimProvider;
	}
	
	AssertIsNotNull(aimProvider)
	
	_aimProvider = aimProvider;
	Changed.Broadcast();
}
