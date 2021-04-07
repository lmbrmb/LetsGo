#include "FragLimitOptionParser.h"

FragLimitOptionParser::FragLimitOptionParser()
{
}

bool FragLimitOptionParser::TryParseOption(const FString& option, AMatchGameMode* matchGameMode)
{
	FString optionValue;
	if (!TryGetOptionValue(option, "FragLimit", optionValue))
	{
		return false;
	}

	auto const fragLimit = FCString::Atoi(*optionValue);
	matchGameMode->SetFragLimit(fragLimit);
	return true;
}
