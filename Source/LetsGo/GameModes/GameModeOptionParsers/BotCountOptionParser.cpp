#include "BotCountOptionParser.h"

BotCountOptionParser::BotCountOptionParser()
{
}

bool BotCountOptionParser::TryParseOption(const FString& option, AMatchGameMode* matchGameMode)
{
	FString optionValue;
	if (!TryGetOptionValue(option, "BotCount", optionValue))
	{
		return false;
	}

	auto const botCount = FCString::Atoi(*optionValue);
	matchGameMode->SetBotCount(botCount);
	return true;
}
