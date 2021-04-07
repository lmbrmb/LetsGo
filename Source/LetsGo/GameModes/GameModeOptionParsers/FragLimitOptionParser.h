#pragma once
#include "IGameModeOptionParser.h"

class FragLimitOptionParser final : public IGameModeOptionParser
{
public:
	FragLimitOptionParser();

	virtual bool TryParseOption(const FString& option, AMatchGameMode* matchGameMode) override;
};
