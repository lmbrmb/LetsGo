#include "MappingComponent.h"

UMappingComponent::UMappingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMappingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Map();
}

// HACK:
// Call of DestroyComponent() in BeginPlay() leads to "Failed to route BeginPlay" error
// This function delays DestroyComponent() call
// Don't like this implementation because of timer
void UMappingComponent::StartDestroyTask()
{
	FTimerHandle destroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle, this, &UMappingComponent::DestroyOnTimer, 5, false);
}

void UMappingComponent::DestroyOnTimer()
{
	DestroyComponent();
}
