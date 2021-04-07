#include "LocalPlayerSkinIdOptionParser.h"
#include "LetsGo/Data/SkinId.h"

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

	const FName localPlayerSkinIdValue(optionValue);
	const SkinId localPlayerSkinId(localPlayerSkinIdValue);
	matchGameMode->SetLocalPlayerSkinId(localPlayerSkinId);
	return true;
}
