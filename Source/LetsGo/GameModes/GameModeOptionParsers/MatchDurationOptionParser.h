#pragma once

#include "IGameModeOptionParser.h"

class MatchDurationOptionParser final : public IGameModeOptionParser
{
public:
	MatchDurationOptionParser();

	virtual bool TryParseOption(const FString& option, AMatchGameMode* matchGameMode) override;
};
