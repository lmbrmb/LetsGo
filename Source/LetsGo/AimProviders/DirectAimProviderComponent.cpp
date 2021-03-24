#include "DirectAimProviderComponent.h"

#include "DirectAimProvider.h"

void UDirectAimProviderComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const directAimProvider = new DirectAimProvider();
	SetAimProvider(directAimProvider);
}
