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

	if(!_pickupItem)
	{
		return;
	}

	if(_pickupFirstSpawnDelay > 0)
	{
		DeactivatePickup();
		RequestSpawnPickupAfterDelay(_pickupFirstSpawnDelay);
	}
	else
	{
		SpawnPickup();
	}
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
	AssertIsNotNull(pickupItemBlueprintGeneratedClass);
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
	ActivatePickup();
	// Actual spawn time
	_pickupSpawnTime = GetWorld()->TimeSeconds;
	BpOnPickupSpawn();
}

void APickupSpawnPoint::OnPickupTaken(APickupItem* pickupItem)
{
	DeactivatePickup();
	RequestSpawnPickupAfterDelay(_pickupRespawnInterval);
}

void APickupSpawnPoint::RequestSpawnPickupAfterDelay(const float delay)
{
	AssertIsGreater(delay, 0.0f);
	GetWorld()->GetTimerManager().SetTimer(_pickupRespawnTimerHandle, this, &APickupSpawnPoint::SpawnPickup, delay, false);
	// Approximate spawn time
	_pickupSpawnTime = GetWorld()->TimeSeconds + _pickupRespawnInterval;
}

void APickupSpawnPoint::ActivatePickup() const
{
	ActorUtils::SetEnabled(_pickupItem, true);
}

void APickupSpawnPoint::DeactivatePickup() const
{
	ActorUtils::SetEnabled(_pickupItem, false);
}
