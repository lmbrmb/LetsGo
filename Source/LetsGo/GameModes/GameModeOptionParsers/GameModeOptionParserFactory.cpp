#include "GameModeOptionParserFactory.h"

#include "BotCountOptionParser.h"
#include "FragLimitOptionParser.h"
#include "LocalPlayerSkinIdOptionParser.h"
#include "MatchDurationOptionParser.h"
#include "LetsGo/Utils/AssertUtils.h"

GameModeOptionParserFactory::GameModeOptionParserFactory()
{
	_optionParsers.Add(new BotCountOptionParser());
	_optionParsers.Add(new FragLimitOptionParser());
	_optionParsers.Add(new LocalPlayerSkinIdOptionParser());
	_optionParsers.Add(new MatchDurationOptionParser());
}

bool GameModeOptionParserFactory::TryParseOption(const FString& option, AMatchGameMode* matchGameMode)
{
	AssertIsNotNull(matchGameMode, false);
	
	for (auto optionParser : _optionParsers)
	{
		auto const isParsed = optionParser->TryParseOption(option, matchGameMode);
		if (isParsed)
		{
			return true;
		}
	}

	return false;
}
