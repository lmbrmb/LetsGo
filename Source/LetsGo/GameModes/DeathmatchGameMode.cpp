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

	auto const fragLimit = GetFragLimit();
	if(maxFrags >= fragLimit)
	{
		SetMatchState(MatchState::Ended);
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

int ADeathmatchGameMode::CalcPlayerPlace(const PlayerId& playerId) const
{
	auto const playerFragCount = GetPlayerFragCount(playerId);
	
	TArray<int> fragCountDistinct;
	
	for (auto const playerFragRecord : PlayerFrags)
	{
		auto const fragCount = playerFragRecord.Value;
		
		if(fragCountDistinct.Contains(fragCount))
		{
			continue;
		}
		fragCountDistinct.Add(fragCount);
	}

	fragCountDistinct.Sort();
	auto const playerIndexByScore = fragCountDistinct.Find(playerFragCount);
	auto const arrayCount = fragCountDistinct.Num();
	auto const place = arrayCount - playerIndexByScore;
	
	return place;
}

void ADeathmatchGameMode::PopulateAvatarsData()
{
	auto teamIndex = 0;
	auto const botCount = GetBotCount();
	for (auto i = 0; i < botCount; i++)
	{
		auto const botIdValue = INT_MAX - i;
		const PlayerId botId(botIdValue);
		const TeamId teamId(teamIndex++);
		auto const avatarData = _avatarDataFactory->GenerateRandom(botId, AvatarType::Bot, teamId);
		AvatarsData.Add(botIdValue, avatarData);
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
}
