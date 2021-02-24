#include "PickupSpawnPoint.h"
#include "Kismet/KismetStringLibrary.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/PickupItems/PickupItem.h"
#include "LetsGo/Utils/AssetUtils.h"

APickupSpawnPoint::APickupSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	
	_root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(_root);
	
	_spawnPivot = CreateDefaultSubobject<USceneComponent>("SpawnPivot");
	_spawnPivot->SetupAttachment(_root);
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	auto const gameModeBase = dynamic_cast<AMatchGameMode*>(GetWorld()->GetAuthGameMode());
	auto const diContainer = gameModeBase->GetDiContainer();
	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_pickupItemFactory = &pickupItemFactory.Get();
	
	SpawnPickup();
}

void APickupSpawnPoint::SpawnPickup()
{
	if(UKismetStringLibrary::IsEmpty(_id.ToString()))
	{
		DevLogger::GetLoggingChannel()->Log("Item ID is empty");
		return;
	}

	auto const pickupBlueprint =_pickupItemFactory->GetBlueprint(_id);

	if(pickupBlueprint == nullptr)
	{
		DevLogger::GetLoggingChannel()->Log("Item blueprint is not found");
		return;
	}
	
	auto const pickupItem = AssetUtils::SpawnBlueprint<APickupItem>(GetWorld(), this, pickupBlueprint);
	pickupItem->AttachToComponent(_spawnPivot, FAttachmentTransformRules::KeepRelativeTransform);
}
