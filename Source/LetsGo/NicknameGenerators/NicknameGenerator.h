#pragma once 

#include "INicknameGenerator.h"

/// <summary>
/// Nickname generator
/// </summary>
class NicknameGenerator final : public INicknameGenerator
{
public:
	explicit NicknameGenerator(const float predefinedNicknameProbability);

	virtual FName Generate() override;

protected:
	~NicknameGenerator() = default;

private:
	float _predefinedNicknameProbability;
	
	TArray<FString> _predefinedNicknames;
	
	TArray<FString> _nicknamePrefixes;

	TArray<FString> _nicknameNouns;

	TArray<FString> _nicknamePostfixes;

	TArray<FString> _generatedNames;

	FString TakeRandom(TArray<FString>& array) const;
};
