#include "MatchGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/DiContainers/MatchDiContainerFactory.h"
#include "LetsGo/Utils/ActorUtils.h"
#include "LetsGo/Utils/AssertUtils.h"

AMatchGameMode::~AMatchGameMode()
{
	delete _diContainer;
	delete _matchAnalytics;
	delete _playerSettingsManager;
	delete _matchSettingsManager;
}

void AMatchGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MatchDiContainerFactory<ESPMode::Fast> containerFactory;
	_diContainer = containerFactory.CreateContainer(this);

	auto const avatarSpawnFactory = GetDiContainer()->GetInstance<AvatarSpawnFactory>();
	_avatarSpawnFactory = &avatarSpawnFactory.Get();

	auto const settingsManagerFactoryRef = GetDiContainer()->GetInstance<SettingsManagerFactory>();
	auto const settingsManagerFactory = &settingsManagerFactoryRef.Get();

	_playerSettingsManager = settingsManagerFactory->Create<PlayerSettingsManager>(_playerSettingsSlotName, true);
	_matchSettingsManager = settingsManagerFactory->Create<MatchSettingsManager>(_matchSettingsSlotName, true);

	if (_applySavedSettings)
	{
		auto const playerSettings = _playerSettingsManager->GetSettings();
		auto const localPlayerSkinId = playerSettings->PlayerSkinId;
		_localPlayerSkinId = SkinId(localPlayerSkinId);

		auto const matchSettings = _matchSettingsManager->GetSettings();
		_botCount = matchSettings->BotCount;
		_fragLimit = matchSettings->FragLimit;
	}

	auto const matchAnalyticsFactoryRef = GetDiContainer()->GetInstance<MatchAnalyticsFactory>();
	auto const matchAnalyticsFactory = &matchAnalyticsFactoryRef.Get();

	_matchAnalytics = matchAnalyticsFactory->Create(this);
}

void AMatchGameMode::TriggerMatchWarmUp()
{
	if(_warmUpDuration > 0)
	{
		SetMatchState(MatchState::WarmUp);
		GetWorldTimerManager().SetTimer(_matchStateTimerHandle, this, &AMatchGameMode::TriggerMatchStart, _warmUpDuration, false);
		return;
	}

	TriggerMatchStart();
}

void AMatchGameMode::TriggerMatchStart()
{
	SetMatchState(MatchState::Started);

	if(_matchDuration > 0)
	{
		GetWorldTimerManager().SetTimer(_matchStateTimerHandle, this, &AMatchGameMode::TriggerMatchEnd, _matchDuration, false);
	}
}

void AMatchGameMode::TriggerMatchEnd()
{
	SetMatchState(MatchState::Ended);

	GetWorldTimerManager().ClearTimer(_matchStateTimerHandle);

	if(_exitToMainMenuDelay > 0)
	{
		FTimerHandle exitToMainMenuTimerHandle;
		GetWorldTimerManager().SetTimer(exitToMainMenuTimerHandle, this, &AMatchGameMode::TriggerExitToMainMenu, _exitToMainMenuDelay, false);
		return;
	}

	TriggerExitToMainMenu();
}

void AMatchGameMode::TriggerExitToMainMenu()
{
	GetWorldTimerManager().ClearTimer(_matchStateTimerHandle);
	ExitToMainMenu();
}

AvatarData* AMatchGameMode::GetAvatarData(const int playerIdValue) const
{
	if(AvatarsData.Contains(playerIdValue))
	{
		return AvatarsData[playerIdValue];
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
	
	for (auto const avatarDataEntry : AvatarsData)
	{
		auto const avatarData = avatarDataEntry.Value;
		auto const playerIdValue = avatarData->GetPlayerId().GetId();
		PlayerFrags.Add(playerIdValue, 0);

		auto const teamIdValue = avatarData->GetTeamId().GetId();
		if(!TeamFrags.Contains(teamIdValue))
		{
			TeamFrags.Add(teamIdValue, 0);
		}

		SpawnAvatar(avatarData);
	}

	TriggerMatchWarmUp();
}

void AMatchGameMode::RegisterSpawnPoint(const FSpawnPointType type,  const FTransform& transform)
{
	AssertIsGreater((int)type, 0);

	switch (type)
	{
		case FSpawnPointType::Player:
			_playerSpawnPoints.Add(transform);
			break;
		case FSpawnPointType::Health:
			_healthSpawnPoints.Add(transform);
			break;
		case FSpawnPointType::Weapon:
			_weaponSpawnPoints.Add(transform);
			break;
		default:
			DevLogger::GetLoggingChannel()->LogValue("Unhandled spawn point type:", (int)type, LogSeverity::Error);
			break;
	}
}

float AMatchGameMode::GetMatchTime() const
{
	if(_matchState == MatchState::Ended)
	{
		return _matchEndTime;
	}
	
	return GetCurrentStateTime();
}

bool AMatchGameMode::IsMatchWarmUp() const
{
	return _matchState == MatchState::WarmUp;
}

bool AMatchGameMode::IsMatchStarted() const
{
	return static_cast<int>(_matchState) >= static_cast<int>(MatchState::Started);
}

bool AMatchGameMode::IsMatchEnded() const
{
	return _matchState == MatchState::Ended;
}

bool AMatchGameMode::IsMatchInProgress() const
{
	return _matchState == MatchState::Started;
}

float AMatchGameMode::GetCurrentStateTime() const
{
	return GetWorld()->TimeSeconds - _stateStartTime;
}

FTransform AMatchGameMode::GetNextSpawnPoint()
{
	auto const spawnPointCount = _playerSpawnPoints.Num();
	
	if(spawnPointCount <= 0)
	{
		DevLogger::GetLoggingChannel()->Log("No spawn points registered", LogSeverity::Error);
		return FTransform();
	}
	
	auto nextIndex = _playerSpawnPointIndex + 1;

	if (nextIndex >= spawnPointCount)
	{
		nextIndex = 0;
	}

	_playerSpawnPointIndex = nextIndex;
	return _playerSpawnPoints[_playerSpawnPointIndex];
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
	_avatars.Add(avatar);
}

void AMatchGameMode::RespawnAvatarOnTimer()
{
	int playerIdValue;
	_respawnQueue.Dequeue(playerIdValue);

	for (auto const avatarDataEntry : AvatarsData)
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
	AAvatar* actor;
	_avatarDestroyQueue.Dequeue(actor);
	ActorUtils::DestroyActorRecursively(actor);
	_avatars.Remove(actor);
}

void AMatchGameMode::OnAvatarDied(UHealthComponent* healthComponent, const float delta)
{
	// Timers are in fire-and-forget mode
	FTimerHandle destroyTimerHandle;
	GetWorldTimerManager().SetTimer(destroyTimerHandle, this, &AMatchGameMode::DestroyAvatarOnTimer, _avatarDestroyTime, false);

	auto const lastDamage = healthComponent->GetLastDamage();
	auto const instigatorPlayerId = lastDamage.GetInstigatorPlayerId();
	auto const instigatorPlayerIdValue = instigatorPlayerId.GetId();
	
	auto const fraggedPlayerActor = healthComponent->GetOwner();
	auto const fraggedPlayerAvatar = Cast<AAvatar>(fraggedPlayerActor);

	AssertIsNotNull(fraggedPlayerAvatar);

	_avatarDestroyQueue.Enqueue(fraggedPlayerAvatar);
	
	auto const fraggedPlayerId = fraggedPlayerAvatar->GetPlayerId();
	auto const fraggedPlayerIdValue = fraggedPlayerId.GetId();

	auto const fraggedByPlayer = lastDamage.GetInstigatorPlayerId().IsValid();
	
	if(fraggedByPlayer)
	{
		auto const instigatorPlayerAvatarData = GetAvatarData(instigatorPlayerIdValue);
		auto const fraggedPlayerAvatarData = GetAvatarData(fraggedPlayerIdValue);

		if (instigatorPlayerAvatarData && fraggedPlayerAvatarData)
		{
			if (IsMatchInProgress())
			{
				auto const isSuicide = instigatorPlayerIdValue == fraggedPlayerIdValue;
				auto const fragModifier = isSuicide ? -1 : 1;

				PlayerFrags[instigatorPlayerIdValue] = PlayerFrags[instigatorPlayerIdValue] + fragModifier;
				auto const instigatorPlayerTeamIdValue = instigatorPlayerAvatarData->GetTeamId().GetId();
				TeamFrags[instigatorPlayerTeamIdValue] = TeamFrags[instigatorPlayerTeamIdValue] + fragModifier;
				OnFragsCountChanged();
			}

			PlayerFragged.Broadcast(instigatorPlayerId, fraggedPlayerId);
		}
	}
	
	_respawnQueue.Enqueue(fraggedPlayerIdValue);
	FTimerHandle respawnTimerHandle;
	GetWorldTimerManager().SetTimer(respawnTimerHandle, this, &AMatchGameMode::RespawnAvatarOnTimer, _avatarRespawnTime, false);
}

void AMatchGameMode::SetMatchState(MatchState matchState)
{
	GetWorldTimerManager().ClearTimer(_matchStateTimerHandle);
	
	if(!CanEnterState(matchState))
	{
		auto const loggingChannel = DevLogger::GetLoggingChannel();
		loggingChannel->Log("Can't set match state", LogSeverity::Error);
		loggingChannel->LogValue("Current state:", (int)_matchState, LogSeverity::Error);
		loggingChannel->LogValue("New match state:", (int)matchState, LogSeverity::Error);
		return;
	}

	_matchState = matchState;

	switch (_matchState)
	{
		case MatchState::WarmUp:
			MatchWarmUp.Broadcast();
			break;
		case MatchState::Started:
			MatchStart.Broadcast();
			break;
		case MatchState::Ended:
			_matchEndTime = GetCurrentStateTime();
			MatchEnd.Broadcast();
			break;
		default:
			//Do nothing
			break;
	}
	
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

// Template methods below
void AMatchGameMode::PopulateAvatarsData()
{
	AssertDefaultImplementationIsOverriden();
}

void AMatchGameMode::OnFragsCountChanged()
{
	AssertDefaultImplementationIsOverriden();
}

bool AMatchGameMode::IsLocalPlayerWonMatch()
{
	AssertDefaultImplementationIsOverriden(false);
}

int AMatchGameMode::GetFragLimit() const
{
	return _fragLimit;
}

int AMatchGameMode::GetBotCount() const
{
	return _botCount;
}

const SkinId& AMatchGameMode::GetLocalPlayerSkinId() const
{
	return _localPlayerSkinId;
}

float AMatchGameMode::GetMatchDuration() const
{
	return _matchDuration;
}

int AMatchGameMode::GetPlayerPlace(const PlayerId& playerId) const
{
	AssertDefaultImplementationIsOverriden(0);
}

int AMatchGameMode::GetPlayerFragCount(const PlayerId& playerId) const
{
	auto const playerIdValue = playerId.GetId();
	AssertContainerContainsElement(PlayerFrags, playerIdValue, 0);
	
	return PlayerFrags[playerIdValue];
}

int AMatchGameMode::GetTeamFragCount(const TeamId& teamId) const
{
	auto const teamIdValue = teamId.GetId();
	AssertContainerContainsElement(TeamFrags, teamIdValue, 0);

	return TeamFrags[teamIdValue];
}

TeamId AMatchGameMode::GetPlayerTeamId(const PlayerId& playerId) const
{
	auto const avatarData = GetAvatarData(playerId);
	if(avatarData)
	{
		return avatarData->GetTeamId();
	}

	return TeamId();
}

FName AMatchGameMode::GetPlayerNickname(const PlayerId& playerId) const
{
	auto const avatarData = GetAvatarData(playerId);
	if (avatarData)
	{
		return avatarData->GetNickname();
	}

	return FName();
}

const TArray<AAvatar*>& AMatchGameMode::GetAvatars() const
{
	return _avatars;
}

 UPlayerSettings* AMatchGameMode::GetPlayerSettings() const
{
	 return _playerSettingsManager->GetSettings();
}

UMatchSettings* AMatchGameMode::GetMatchSettings() const
{
	return _matchSettingsManager->GetSettings();
}

const TMap<int, int>& AMatchGameMode::GetPlayerFrags() const
{
	return PlayerFrags;
}

const TMap<int, int>& AMatchGameMode::GetTeamFrags() const
{
	return TeamFrags;
}

void AMatchGameMode::ExitToMainMenu() const
{
	Exit.Broadcast();
	UGameplayStatics::OpenLevel(this, _mainMenuLevelName, true);
}
