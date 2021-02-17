#include "MappingComponent.h"

UMappingComponent::UMappingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMappingComponent::BeginPlay()
{
	Super::BeginPlay();
	Map();
	if(ShouldDestroyAfterMapping())
	{
		DestroyComponent();
	}
}

bool UMappingComponent::ShouldDestroyAfterMapping() const
{
	return true;
}
