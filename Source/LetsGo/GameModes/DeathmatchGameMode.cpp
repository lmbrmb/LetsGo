#include "DeathmatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "LetsGo/Utils/AssertUtils.h"

const FName LOCAL_PLAYER_NAME = "%UserName%";

const FName LOCAL_PLAYER_SKIN_ID = "Kachujin";

void ADeathmatchGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	ParseMatchOptions(Options);

	auto const avatarDataFactory = GetDiContainer()->GetInstance<AvatarDataFactory>();
	_avatarDataFactory = &avatarDataFactory.Get();
}

void ADeathmatchGameMode::ParseMatchOptions(const FString& options)
{
	auto stringToSplit = options;
	FString option, remainder;

	TArray<TFunction<bool(const FString&)>> optionParsers;
	optionParsers.Add([this](auto option) { return this->TryParseBotCountOption(option); });
	optionParsers.Add([this](auto option) { return this->TryParseFragLimitOption(option); });
	
	while (stringToSplit.Split(TEXT(";"), &option, &remainder))
	{
		stringToSplit = remainder;

		for (auto optionParser : optionParsers)
		{
			auto const isParsed = optionParser(option);
			if(isParsed)
			{
				break;
			}
		}
	}
}

bool ADeathmatchGameMode::TryParseBotCountOption(const FString& option)
{
	FString optionValue;
	if (!TryGetOptionValue(option, "BotCount", optionValue))
	{
		return false;
	}

	auto const botCount = FCString::Atoi(*optionValue);
	_botCount = botCount;
	return true;
}

bool ADeathmatchGameMode::TryParseFragLimitOption(const FString& option)
{
	FString optionValue;
	if(!TryGetOptionValue(option, "FragLimit", optionValue))
	{
		return false;
	}

	auto const fragLimit = FCString::Atoi(*optionValue);
	_fragLimit = fragLimit;
	return true;
}

bool ADeathmatchGameMode::TryGetOptionValue(
	const FString& option,
	const FString& optionName,
	FString& outOptionValue
)
{
	auto const optionKey = optionName + "=";
	if (option.Find( optionKey) == -1)
	{
		return false;
	}

	FString left, right;
	option.Split(TEXT("="), &left, &outOptionValue);
	return true;
}

void ADeathmatchGameMode::OnFragsCountChanged()
{
	auto maxFrags = MIN_int32;
	auto playerWithMaxFrags = -1;
	
	for (auto const frag : Frags)
	{
		if(maxFrags < frag.Value)
		{
			maxFrags = frag.Value;
			playerWithMaxFrags = frag.Key;
		}
	}

	_winnerPlayerId = PlayerId(playerWithMaxFrags);

	if(maxFrags >= _fragLimit)
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
	for (auto i = 0; i < _botCount; i++)
	{
		auto const botIdValue = MAX_int32 - i;
		const PlayerId botId(botIdValue);
		auto const avatarData = _avatarDataFactory->GenerateRandom(botId, AvatarType::Bot, teamIndex++);
		AvatarsData.Add(botIdValue, avatarData);
	}

	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AssertIsNotNull(localPlayerController);

	auto const localPlayerState = localPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(localPlayerState);

	auto const localPlayerIdValue = localPlayerState->GetPlayerId();
	_localPlayerId = PlayerId(localPlayerIdValue);
	auto const avatarData = _avatarDataFactory->Create(
		_localPlayerId,
		AvatarType::LocalPlayer,
		LOCAL_PLAYER_SKIN_ID,
		LOCAL_PLAYER_NAME,
		teamIndex++
	);
	AvatarsData.Add(localPlayerIdValue, avatarData);
}
