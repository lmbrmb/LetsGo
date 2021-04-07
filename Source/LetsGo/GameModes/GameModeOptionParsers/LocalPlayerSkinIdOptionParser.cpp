#include "LocalPlayerSkinIdOptionParser.h"

LocalPlayerSkinIdOptionParser::LocalPlayerSkinIdOptionParser()
{
}

bool LocalPlayerSkinIdOptionParser::TryParseOption(const FString& option, AMatchGameMode* matchGameMode)
{
	FString optionValue;
	if (!TryGetOptionValue(option, "LocalPlayerSkinId", optionValue))
	{
		return false;
	}

	auto const localPlayerSkinId = FName(optionValue);
	matchGameMode->SetLocalPlayerSkinId(localPlayerSkinId);
	return true;
}
