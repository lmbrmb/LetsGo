#pragma once

#include "IGameModeOptionParser.h"

class LocalPlayerSkinIdOptionParser final : public IGameModeOptionParser
{
public:
	LocalPlayerSkinIdOptionParser();

	virtual bool TryParseOption(const FString& option, AMatchGameMode* matchGameMode) override;
};
