#include "PickupSpawnPoint.h"
#include "Kismet/KismetStringLibrary.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/PickupItems/PickupItem.h"
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

	auto const gameModeBase = dynamic_cast<AMatchGameMode*>(GetWorld()->GetAuthGameMode());
	auto const diContainer = gameModeBase->GetDiContainer();
	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_pickupItemBlueprint = pickupItemFactory->GetBlueprint(_id);
	AssertIsNotNull(_pickupItemBlueprint);

	_timerDelegate.BindUFunction(this, FName("RespawnPickupOnTimer"));

	SpawnPickup();
}

void APickupSpawnPoint::SpawnPickup()
{
	auto const pickupItem = AssetUtils::SpawnBlueprint<APickupItem>(GetWorld(), this, _pickupItemBlueprint);
	pickupItem->AttachToComponent(_spawnPivot, FAttachmentTransformRules::KeepRelativeTransform);
	_delegateHandleOnPickupTaken = pickupItem->Taken.AddUObject(this, &APickupSpawnPoint::OnPickupTaken);
}

void APickupSpawnPoint::OnPickupTaken(APickupItem* pickupItem)
{
	pickupItem->Taken.Remove(_delegateHandleOnPickupTaken);
	_delegateHandleOnPickupTaken.Reset();

	auto world = GetWorld();
	
	GetWorldTimerManager().SetTimer(_timerHandle, _timerDelegate, _pickupRespawnTime, false);
}

void APickupSpawnPoint::RespawnPickupOnTimer()
{
	SpawnPickup();
}
