#include "PickupSpawnPoint.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Pickups/PickupItem.h"
#include "LetsGo/AssetFactories/PickupItemFactory.h"
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

float APickupSpawnPoint::GetPickupSpawnTime() const
{
	return _pickupSpawnTime;
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	AssertStringIsNotEmpty(_id);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();
	
	auto const pickupItemFactory = diContainer->GetInstance<PickupItemFactory>();
	_pickupItemBlueprintGeneratedClass = pickupItemFactory->GetOrLoad(_id);
	AssertIsNotNull(_pickupItemBlueprintGeneratedClass);

	matchGameMode->RegisterSpawnPoint(_spawnPointType, GetTransform());

	SpawnPickup();
}

void APickupSpawnPoint::SpawnPickup()
{
	auto const pickupItem = AssetUtils::SpawnBlueprint<APickupItem>(
		GetWorld(),
		this,
		_pickupItemBlueprintGeneratedClass,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	AssertIsNotNull(pickupItem);
	//Tricky: Subscribe first, then attach to pivot because pickup can be taken on spawn
	_delegateHandleOnPickupTaken = pickupItem->Taken.AddUObject(this, &APickupSpawnPoint::OnPickupTaken);
	pickupItem->AttachToComponent(_spawnPivot, FAttachmentTransformRules::KeepRelativeTransform);
	_pickupSpawnTime = GetWorld()->TimeSeconds;
}

void APickupSpawnPoint::OnPickupTaken(APickupItem* pickupItem)
{
	pickupItem->Taken.Remove(_delegateHandleOnPickupTaken);
	_delegateHandleOnPickupTaken.Reset();

	GetWorldTimerManager().SetTimer(_timerHandle, this, &APickupSpawnPoint::RespawnPickupOnTimer, _pickupRespawnInterval, false);
	_pickupSpawnTime = GetWorld()->TimeSeconds + _pickupRespawnInterval;
}

void APickupSpawnPoint::RespawnPickupOnTimer()
{
	SpawnPickup();
}
