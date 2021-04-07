#pragma once
#include "IGameModeOptionParser.h"

class GameModeOptionParserFactory final
{
public:
	GameModeOptionParserFactory();

	bool TryParseOption(const FString& option, AMatchGameMode* matchGameMode);
	
	TArray<IGameModeOptionParser*> _optionParsers;
};

Expose_TNameOf(GameModeOptionParserFactory)