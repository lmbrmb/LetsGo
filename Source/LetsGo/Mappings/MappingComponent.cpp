#include "MappingComponent.h"

UMappingComponent::UMappingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMappingComponent::BeginPlay()
{
	Super::BeginPlay();
	Map();
	DestroyComponent();
}
