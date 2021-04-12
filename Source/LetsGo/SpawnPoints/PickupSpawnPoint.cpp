#include "PickupSpawnPoint.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Pickups/PickupItem.h"
#include "LetsGo/Utils/ActorUtils.h"
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
	_pickupItemFactory = &pickupItemFactory.Get();
	AssertIsNotNull(_pickupItemFactory);

	matchGameMode->RegisterSpawnPoint(_spawnPointType, GetTransform());

	CreatePickup();
	SpawnPickup();
}

void APickupSpawnPoint::BeginDestroy()
{
	Super::BeginDestroy();

	if(_pickupRespawnTimerHandle.IsValid())
	{
		auto const world = GetWorld();
		if (world)
		{
			world->GetTimerManager().ClearTimer(_pickupRespawnTimerHandle);
		}
	}
}

void APickupSpawnPoint::CreatePickup()
{
	auto const pickupItemBlueprintGeneratedClass = _pickupItemFactory->GetOrLoad(_id);
	_pickupItem = AssetUtils::SpawnBlueprint<APickupItem>(
		GetWorld(),
		this,
		pickupItemBlueprintGeneratedClass,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	AssertIsNotNull(_pickupItem);
	_delegateHandleOnPickupTaken = _pickupItem->Taken.AddUObject(this, &APickupSpawnPoint::OnPickupTaken);
	_pickupItem->AttachToComponent(_spawnPivot, FAttachmentTransformRules::KeepRelativeTransform);
}

void APickupSpawnPoint::SpawnPickup()
{
	ActorUtils::SetEnabled(_pickupItem, true);
	// Actual spawn time
	_pickupSpawnTime = GetWorld()->TimeSeconds;
}

void APickupSpawnPoint::OnPickupTaken(APickupItem* pickupItem)
{
	ActorUtils::SetEnabled(_pickupItem, false);
	GetWorld()->GetTimerManager().SetTimer(_pickupRespawnTimerHandle, this, &APickupSpawnPoint::RespawnPickupOnTimer, _pickupRespawnInterval, false);
	// Approximate spawn time
	_pickupSpawnTime = GetWorld()->TimeSeconds + _pickupRespawnInterval;
}

void APickupSpawnPoint::RespawnPickupOnTimer()
{
	SpawnPickup();
}
