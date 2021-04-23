#include "DeathmatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "LetsGo/Utils/AssertUtils.h"

void ADeathmatchGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto const avatarDataFactory = GetDiContainer()->GetInstance<AvatarDataFactory>();
	_avatarDataFactory = &avatarDataFactory.Get();
}

void ADeathmatchGameMode::OnFragsCountChanged()
{
	auto maxFrags = MIN_int32;
	auto playerWithMaxFrags = -1;
	
	for (auto const playerFragRecord : PlayerFrags)
	{
		if(maxFrags < playerFragRecord.Value)
		{
			maxFrags = playerFragRecord.Value;
			playerWithMaxFrags = playerFragRecord.Key;
		}
	}

	_winnerPlayerId = PlayerId(playerWithMaxFrags);

	UpdatePlayerPlaces();

	auto const fragLimit = GetFragLimit();
	if(maxFrags >= fragLimit)
	{
		SetMatchState(MatchState::Ended);
	}
}

void ADeathmatchGameMode::UpdatePlayerPlaces()
{
	auto copy = PlayerFrags;
	copy.ValueSort([](auto a, auto b) {return a > b; });

	TArray<int> playerIds;
	copy.GenerateKeyArray(playerIds);

	_playerPlaces.Empty();
	auto const playerIdsCount = playerIds.Num();

	for (auto i = 0; i < playerIdsCount; i++)
	{
		auto const playerId = playerIds[i];
		auto const place = i + 1;
		_playerPlaces.Add(playerId, place);
	}
}

bool ADeathmatchGameMode::IsLocalPlayerWonMatch()
{
	if (IsMatchInProgress())
	{
		return false;
	}

	return _winnerPlayerId == _localPlayerId;
}

int ADeathmatchGameMode::GetPlayerPlace(const PlayerId& playerId) const
{
	auto const playerIdValue = playerId.GetId();
	AssertContainerContainsElement(_playerPlaces, playerIdValue, -1);
	
	return _playerPlaces[playerIdValue];
}

void ADeathmatchGameMode::PopulateAvatarsData()
{
	auto teamIndex = 0;
	auto const botCount = GetBotCount();
	auto place = 1;
	for (auto i = 0; i < botCount; i++)
	{
		auto const botIdValue = INT_MAX - i;
		const PlayerId botId(botIdValue);
		const TeamId teamId(teamIndex++);
		auto const avatarData = _avatarDataFactory->GenerateRandom(botId, AvatarType::Bot, teamId);
		AvatarsData.Add(botIdValue, avatarData);
		_playerPlaces.Add(botIdValue, place++);
	}

	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AssertIsNotNull(localPlayerController);

	auto const localPlayerState = localPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(localPlayerState);

	auto const localPlayerIdValue = localPlayerState->GetPlayerId();
	_localPlayerId = PlayerId(localPlayerIdValue);
	const TeamId teamId(teamIndex++);
	auto const localPlayerSkinId = GetLocalPlayerSkinId();
	auto const avatarData = _avatarDataFactory->Create(
		_localPlayerId,
		AvatarType::LocalPlayer,
		localPlayerSkinId,
		LOCAL_PLAYER_NAME,
		teamId
	);
	AvatarsData.Add(localPlayerIdValue, avatarData);
	_playerPlaces.Add(localPlayerIdValue, place++);
}
