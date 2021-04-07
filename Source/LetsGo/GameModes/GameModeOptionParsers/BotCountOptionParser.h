#pragma once

#include "IGameModeOptionParser.h"

class BotCountOptionParser final : public IGameModeOptionParser
{
public:
	BotCountOptionParser();

	virtual bool TryParseOption(const FString& option, AMatchGameMode* matchGameMode) override;
};
