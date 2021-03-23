#include "MatchGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/DiContainers/MatchDiContainerFactory.h"
#include "LetsGo/Utils/ActorUtils.h"
#include "LetsGo/Utils/AssertUtils.h"

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

	auto const avatarSpawnFactory = GetDiContainer()->GetInstance<AvatarSpawnFactory>();
	_avatarSpawnFactory = &avatarSpawnFactory.Get();
	
	_matchAnalytics = new MatchAnalytics(this);
}

void AMatchGameMode::TriggerMatchWarmUp()
{
	SetMatchState(MatchState::WarmUp);
	GetWorldTimerManager().SetTimer(_matchStateTimerHandle, this, &AMatchGameMode::TriggerMatchStart, _warmUpDuration, false);
}

void AMatchGameMode::TriggerMatchStart()
{
	SetMatchState(MatchState::Started);
	GetWorldTimerManager().SetTimer(_matchStateTimerHandle, this, &AMatchGameMode::TriggerMatchEnd, _matchDuration, false);
}

void AMatchGameMode::TriggerMatchEnd()
{
	SetMatchState(MatchState::Ended);
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
	
	for (const auto avatarDataEntry : AvatarsData)
	{
		auto const avatarData = avatarDataEntry.Value;
		auto const playerIdValue = avatarData->GetPlayerId().GetId();
		Frags.Add(playerIdValue, 0);
		SpawnAvatar(avatarData);
	}

	GetWorldTimerManager().SetTimer(_matchStateTimerHandle, this, &AMatchGameMode::TriggerMatchWarmUp, _warmUpDelay, false);
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

	for (const auto avatarDataEntry : AvatarsData)
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

		if(IsMatchInProgress())
		{
			auto const isSuicide = instigatorPlayerIdValue == fraggedPlayerIdValue;
			auto const fragModifier = isSuicide ? -1 : 1;

			Frags[instigatorPlayerIdValue] = Frags[instigatorPlayerIdValue] + fragModifier;
			OnFragsCountChanged();
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
	AssertDefaultImplementationIsOverriden();
	return false;
}
