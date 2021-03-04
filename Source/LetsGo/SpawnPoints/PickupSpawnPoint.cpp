#include "PickupSpawnPoint.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/PickupItems/PickupItem.h"
#include "LetsGo/PickupItems/PickupItemFactory.h"
#include "LetsGo/Utils/AssertUtils.h"
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

	AssertStringIsNotEmpty(_id);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_pickupItemBlueprint = pickupItemFactory->GetBlueprint(_id);
	
	AssertIsNotNull(_pickupItemBlueprint);

	SpawnPickup();
}

void APickupSpawnPoint::SpawnPickup()
{
	auto const pickupItem = AssetUtils::SpawnBlueprint<APickupItem>(GetWorld(), this, _pickupItemBlueprint);
	AssertIsNotNull(pickupItem);
	//Tricky: Subscribe first, then attach to pivot because pickup can be taken on spawn
	_delegateHandleOnPickupTaken = pickupItem->Taken.AddUObject(this, &APickupSpawnPoint::OnPickupTaken);
	pickupItem->AttachToComponent(_spawnPivot, FAttachmentTransformRules::KeepRelativeTransform);
}

void APickupSpawnPoint::OnPickupTaken(APickupItem* pickupItem)
{
	pickupItem->Taken.Remove(_delegateHandleOnPickupTaken);
	_delegateHandleOnPickupTaken.Reset();

	GetWorldTimerManager().SetTimer(_timerHandle, this, &APickupSpawnPoint::RespawnPickupOnTimer, _pickupRespawnTime, false);
}

void APickupSpawnPoint::RespawnPickupOnTimer()
{
	SpawnPickup();
}
