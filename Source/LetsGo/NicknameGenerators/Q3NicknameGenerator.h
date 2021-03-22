#pragma once 

#include "INicknameGenerator.h"

/// <summary>
/// Q3 Nickname generator
/// </summary>
class Q3NicknameGenerator final : public INicknameGenerator
{
public:
	Q3NicknameGenerator();

	virtual FName Generate() override;

private:
	TArray<FString> _nicknames;
};
