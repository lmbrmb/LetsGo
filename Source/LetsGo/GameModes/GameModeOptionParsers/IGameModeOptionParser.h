#pragma once
#include "LetsGo/GameModes/MatchGameMode.h"

class IGameModeOptionParser
{
public:
	virtual ~IGameModeOptionParser() = default;

	virtual bool TryParseOption(const FString& option, AMatchGameMode* matchGameMode) = 0;

	static bool TryGetOptionValue(
		const FString& option,
		const FString& optionName,
		FString& outOptionValue
	)
	{
		auto const optionKey = optionName + "=";
		if (option.Find(optionKey) == -1)
		{
			return false;
		}

		FString left, right;
		option.Split(TEXT("="), &left, &outOptionValue);
		return true;
	}
};
