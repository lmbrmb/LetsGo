#include "MatchGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/DiContainers/MatchDiContainerFactory.h"
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
	
	MatchDiContainerFactory containerFactory;
	_diContainer = containerFactory.CreateContainer<ESPMode::Fast>();

	auto const avatarDataFactory = GetDiContainer()->GetInstance<AvatarDataFactory>();
	_avatarDataFactory = &avatarDataFactory.Get();

	auto const avatarSpawnFactory = GetDiContainer()->GetInstance<AvatarSpawnFactory>();
	_avatarSpawnFactory = &avatarSpawnFactory.Get();
	
	_matchAnalytics = new MatchAnalytics(this);
}

void AMatchGameMode::PopulateAvatarsData()
{
	for (auto i = 0; i < BOT_COUNT; i++)
	{
		auto const botIdValue = MAX_int32 - i;
		const PlayerId botId(botIdValue);
		auto const avatarData = _avatarDataFactory->GenerateRandom(botId, AvatarType::Bot);
		_avatarsData.Add(botIdValue, avatarData);
	}
	
	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AssertIsNotNull(localPlayerController);

	auto const localPlayerState = localPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(localPlayerState);

	auto const localPlayerIdValue = localPlayerState->GetPlayerId();
	auto const localPlayerId = PlayerId(localPlayerIdValue);
	
	auto const avatarData = _avatarDataFactory->Create(localPlayerId, AvatarType::LocalPlayer, LOCAL_PLAYER_SKIN_ID, LOCAL_PLAYER_NAME);
	_avatarsData.Add(localPlayerIdValue, avatarData);
}

void AMatchGameMode::TriggerMatchStart()
{
	SetMatchState(MatchState::Started);

	MatchStarted.Broadcast();
	GetWorldTimerManager().SetTimer(_matchTimerHandle, this, &AMatchGameMode::TriggerMatchEnd, _warmupDuration, false);
}

void AMatchGameMode::TriggerMatchEnd()
{
	SetMatchState(MatchState::Ended);
}

AvatarData* AMatchGameMode::GetAvatarData(const int playerIdValue) const
{
	if(_avatarsData.Contains(playerIdValue))
	{
		return _avatarsData[playerIdValue];
	}

	DevLogger::GetLoggingChannel()->LogValue("Can't find avatar data. Player id value:", playerIdValue, LogSeverity::Error);
	
	return nullptr;
}

AvatarData* AMatchGameMode::GetAvatarData(const PlayerId& playerIdValue) const
{
	return GetAvatarData(playerIdValue.GetId());
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

	_stateStartTime = GetWorld()->TimeSeconds;
	
	PopulateAvatarsData();

	SetMatchState(MatchState::WarmUp);
	
	for (const auto avatarDataEntry : _avatarsData)
	{
		auto const avatarData = avatarDataEntry.Value;
		SpawnAvatar(avatarData);
	}

	GetWorldTimerManager().SetTimer(_matchTimerHandle, this, &AMatchGameMode::TriggerMatchStart, _warmupDuration, false);
}

void AMatchGameMode::RegisterSpawnPoint(FTransform spawnPoint)
{
	_spawnPoints.Add(spawnPoint);
}

float AMatchGameMode::GetMatchTime() const
{
	if(_matchState == MatchState::Ended)
	{
		return _matchEndTime;
	}
	
	return GetCurrentStateTime();
}

float AMatchGameMode::GetCurrentStateTime() const
{
	return GetWorld()->TimeSeconds - _stateStartTime;
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
	int playerIdValue;
	_respawnQueue.Dequeue(playerIdValue);

	for (const auto avatarDataEntry : _avatarsData)
	{
		auto const avatarData = avatarDataEntry.Value;
		if (avatarData->GetPlayerId().GetId() == playerIdValue)
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

	auto const lastDamage = healthComponent->GetLastDamage();
	auto const instigatorPlayerId = lastDamage.GetInstigatorPlayerId();
	auto const instigatorPlayerIdValue = instigatorPlayerId.GetId();
	
	auto const fraggedPlayerActor = healthComponent->GetOwner();
	auto const fraggedPlayerAvatar = Cast<AAvatar>(fraggedPlayerActor);

	AssertIsNotNull(fraggedPlayerAvatar);
	
	auto const fraggedPlayerId = fraggedPlayerAvatar->GetPlayerId();
	auto const fraggedPlayerIdValue = fraggedPlayerId.GetId();

	auto const instigatorPlayerAvatarData = GetAvatarData(instigatorPlayerIdValue);
	auto const fraggedPlayerAvatarData = GetAvatarData(fraggedPlayerIdValue);

	if(instigatorPlayerAvatarData && fraggedPlayerAvatarData)
	{
		auto const instigatorPlayerNickname = instigatorPlayerAvatarData->GetNickname();
		auto const fraggedPlayerNickname = fraggedPlayerAvatarData->GetNickname();

		PlayerFragged.Broadcast(instigatorPlayerId, fraggedPlayerId, instigatorPlayerNickname, fraggedPlayerNickname);
	}
	
	_respawnQueue.Enqueue(fraggedPlayerIdValue);
	FTimerHandle respawnTimerHandle;
	GetWorldTimerManager().SetTimer(respawnTimerHandle, this, &AMatchGameMode::RespawnAvatarOnTimer, _avatarRespawnTime, false);
}

void AMatchGameMode::SetMatchState(MatchState matchState)
{
	if(!CanEnterState(matchState))
	{
		auto const loggingChannel = DevLogger::GetLoggingChannel();
		loggingChannel->Log("Can't set match state", LogSeverity::Error);
		loggingChannel->LogValue("Current state:", (int)_matchState, LogSeverity::Error);
		loggingChannel->LogValue("New match state:", (int)matchState, LogSeverity::Error);
		return;
	}
	
	if(matchState == MatchState::Ended)
	{
		_matchEndTime = GetCurrentStateTime();
	}

	_matchState = matchState;
	_stateStartTime = GetWorld()->TimeSeconds;
}

bool AMatchGameMode::CanEnterState(MatchState matchState) const
{
	switch (matchState)
	{
		case MatchState::WarmUp:
			return _matchState == MatchState::None;

		case MatchState::Started:
			return _matchState == MatchState::None || _matchState == MatchState::WarmUp;

		case MatchState::Ended:
			return _matchState == MatchState::Started;
		default:
			DevLogger::GetLoggingChannel()->LogValue("Unknown match state:", (int)matchState, LogSeverity::Error);
			return false;
	};
}
