#include "MatchGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/MatchDependencyInjectionContainerFactory.h"
#include "LetsGo/Utils/ActorUtils.h"
#include "LetsGo/Utils/AssertUtils.h"

const int BOT_COUNT = 3;

const FName LOCAL_PLAYER_NAME = "%UserName%";

const FName LOCAL_PLAYER_SKIN_ID = "Kachujin";

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

	auto const avatarDataFactory = GetDiContainer()->GetInstance<AvatarDataFactory>();
	_avatarDataFactory = &avatarDataFactory.Get();

	auto const avatarSpawnFactory = GetDiContainer()->GetInstance<AvatarSpawnFactory>();
	_avatarSpawnFactory = &avatarSpawnFactory.Get();
	
	_matchAnalytics = new MatchAnalytics(this);
}

bool AMatchGameMode::GetIsMatchStarted() const
{
	return _IsMatchStarted;
}

void AMatchGameMode::PopulateAvatarsData()
{
	for (auto i = 0; i < BOT_COUNT; i++)
	{
		auto const avatarData = _avatarDataFactory->GenerateRandom(MAX_int32 - i, AvatarType::Bot);
		_avatarsData.Add(avatarData);
	}
	
	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AssertIsNotNull(localPlayerController);

	auto const localPlayerState = localPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(localPlayerState);

	auto const localPlayerId = localPlayerState->GetPlayerId();
	
	auto const avatarData = _avatarDataFactory->Create(localPlayerId, AvatarType::LocalPlayer, LOCAL_PLAYER_SKIN_ID, LOCAL_PLAYER_NAME);
	_avatarsData.Add(avatarData);
}

void AMatchGameMode::TriggerMatchStart()
{
	_IsMatchStarted = true;
	MatchStarted.Broadcast();
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

	PopulateAvatarsData();
	
	for (auto avatarData : _avatarsData)
	{
		SpawnAvatar(avatarData);
	}

	FTimerHandle matchStartTimerHandle;
	GetWorldTimerManager().SetTimer(matchStartTimerHandle, this, &AMatchGameMode::TriggerMatchStart, _matchStartDelay, false);
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
	auto const avatar = _avatarSpawnFactory->SpawnAvatar(avatarData, GetWorld(), GetNextSpawnPoint());
	if(avatar == nullptr)
	{
		return;
	}
	
	auto const healthComponent = avatar->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);
	healthComponent->Died.AddUObject(this, &AMatchGameMode::OnAvatarDied);

	AvatarSpawned.Broadcast(avatar);
}

void AMatchGameMode::RespawnAvatarOnTimer()
{
	int32 playerId;
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
	ActorUtils::DestroyActorRecursively(actor);
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
