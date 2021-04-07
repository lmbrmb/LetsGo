#pragma once

#include "MatchDurationOptionParser.h"

MatchDurationOptionParser::MatchDurationOptionParser()
{
}

bool MatchDurationOptionParser::TryParseOption(const FString& option, AMatchGameMode* matchGameMode)
{
	FString optionValue;
	if (!TryGetOptionValue(option, "MatchDuration", optionValue))
	{
		return false;
	}

	auto const matchDuration = FCString::Atoi(*optionValue);
	matchGameMode->SetMatchDuration(matchDuration);
	return true;
}
