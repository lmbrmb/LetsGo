#include "PickupSpawnComponent.h"

#include "PickupItem.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/AssetUtils.h"

UPickupSpawnComponent::UPickupSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupSpawnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();

	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_pickupItemFactory = &pickupItemFactory.Get();
	AssertIsNotNull(_pickupItemFactory);
}

void UPickupSpawnComponent::SpawnPickup(const FName& id, const FVector& location)
{
	AssertIsNotNull(_pickupItemFactory);
	auto const pickupItemBlueprintGeneratedClass = _pickupItemFactory->GetOrLoad(id);
	AssertIsNotNull(pickupItemBlueprintGeneratedClass);

	AssetUtils::SpawnBlueprint<APickupItem>(
		GetWorld(),
		nullptr,
		pickupItemBlueprintGeneratedClass,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
		location
		);
}
