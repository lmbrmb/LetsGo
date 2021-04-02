#include "DeathmatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "LetsGo/Utils/AssertUtils.h"

const FName LOCAL_PLAYER_NAME = "%UserName%";

const FName LOCAL_PLAYER_SKIN_ID = "Kachujin";

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
	
	for (auto const frag : PlayerFrags)
	{
		if(maxFrags < frag.Value)
		{
			maxFrags = frag.Value;
			playerWithMaxFrags = frag.Key;
		}
	}

	_winnerPlayerId = PlayerId(playerWithMaxFrags);

	if(maxFrags >= FragLimit)
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

void ADeathmatchGameMode::PopulateAvatarsData()
{
	auto teamIndex = 0;
	for (auto i = 0; i < BotCount; i++)
	{
		auto const botIdValue = MAX_int32 - i;
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
	auto const avatarData = _avatarDataFactory->Create(
		_localPlayerId,
		AvatarType::LocalPlayer,
		LOCAL_PLAYER_SKIN_ID,
		LOCAL_PLAYER_NAME,
		teamId
	);
	AvatarsData.Add(localPlayerIdValue, avatarData);
}
