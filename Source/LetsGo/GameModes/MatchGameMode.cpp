#include "MatchGameMode.h"
#include "LetsGo/MatchDependencyInjectionContainerFactory.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/AssetUtils.h"

const int BOT_COUNT = 3;

AMatchGameMode::~AMatchGameMode()
{
	delete _diContainer;
	delete _matchAnalytics;
}

void AMatchGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	MatchDependencyInjectionContainerFactory containerFactory;
	_diContainer = containerFactory.CreateContainer<ESPMode::Fast>();

	auto const playerFactory = GetDiContainer()->GetInstance<AvatarFactory>();
	_avatarFactory = &playerFactory.Get();

	_avatarsData.Add(new AvatarData(FGuid::NewGuid(), AvatarType::LocalPlayer));
	for(auto i = 0; i<BOT_COUNT; i++)
	{
		_avatarsData.Add(new AvatarData(FGuid::NewGuid(), AvatarType::Bot));
	}

	_matchAnalytics = new MatchAnalytics(this);
}

TTypeContainer<ESPMode::Fast>* AMatchGameMode::GetDiContainer() const
{
	return _diContainer;
}

MatchAnalytics* AMatchGameMode::GetMatchAnalytics() const
{
	return _matchAnalytics;
}

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (auto avatarData : _avatarsData)
	{
		SpawnAvatar(avatarData);
	}
}

void AMatchGameMode::RegisterSpawnPoint(FTransform spawnPoint)
{
	_spawnPoints.Add(spawnPoint);
}

FTransform AMatchGameMode::GetNextSpawnPoint()
{
	auto const spawnPointCount = _spawnPoints.Num();
	
	if(spawnPointCount <= 0)
	{
		DevLogger::GetLoggingChannel()->Log("No spawn points registered", LogSeverity::Error);
		return FTransform();
	}
	
	auto nextIndex = _spawnPointIndex + 1;

	if (nextIndex >= spawnPointCount)
	{
		nextIndex = 0;
	}

	_spawnPointIndex = nextIndex;
	return _spawnPoints[_spawnPointIndex];
}

void AMatchGameMode::SpawnAvatar(AvatarData* avatarData)
{
	auto const avatarType = avatarData->GetAvatarType();
	auto const avatarBlueprint = GetAvatarBlueprint(avatarType);

	if (!avatarBlueprint)
	{
		DevLogger::GetLoggingChannel()->Log("Avatar blueprint is null", LogSeverity::Error);
		return;
	}
	auto const avatar = AssetUtils::SpawnBlueprint<AAvatar>(GetWorld(), nullptr, avatarBlueprint, GetNextSpawnPoint());
	avatar->Init(avatarData->GetPlayerId(), avatarData->GetAvatarType());
	auto const healthComponent = avatar->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);
	healthComponent->Died.AddUObject(this, &AMatchGameMode::OnAvatarDied);

	AvatarSpawned.Broadcast(avatar);
}

UBlueprint* AMatchGameMode::GetAvatarBlueprint(const AvatarType avatarType) const
{
	switch (avatarType)
	{
		case AvatarType::LocalPlayer:
			return  _avatarFactory->GetLocalPlayerBlueprint();

		case AvatarType::RemotePlayer:
			return  _avatarFactory->GetRemotePlayerBlueprint();

		case AvatarType::Bot:
			return _avatarFactory->GetBotBlueprint();
		default:
			return nullptr;
	}
}

void AMatchGameMode::RespawnAvatarOnTimer()
{
	FGuid playerId;
	_respawnQueue.Dequeue(playerId);

	for (auto avatarData : _avatarsData)
	{
		if (avatarData->GetPlayerId() == playerId)
		{
			SpawnAvatar(avatarData);
			break;
		}
	}
}

void AMatchGameMode::DestroyAvatarOnTimer()
{
	AActor* actor;
	_destroyQueue.Dequeue(actor);
	actor->Destroy();
}

void AMatchGameMode::OnAvatarDied(const UHealthComponent* healthComponent, const float delta)
{
	// Timers are in fire-and-forget mode
	
	auto const avatarActor = healthComponent->GetOwner();
	_destroyQueue.Enqueue(avatarActor);
	FTimerHandle destroyTimerHandle;
	GetWorldTimerManager().SetTimer(destroyTimerHandle, this, &AMatchGameMode::DestroyAvatarOnTimer, _avatarDestroyTime, false);
	
	auto const owner = healthComponent->GetOwner();
	auto const avatar = Cast<const AAvatar>(owner);
	AssertIsNotNull(avatar)
	
	auto const playerId = avatar->GetPlayerId();
	_respawnQueue.Enqueue(playerId);
	FTimerHandle respawnTimerHandle;
	GetWorldTimerManager().SetTimer(respawnTimerHandle, this, &AMatchGameMode::RespawnAvatarOnTimer, _avatarRespawnTime, false);
}
