#pragma once 

/// <summary>
/// [Interface] Nickname generator contract
/// </summary>
class INicknameGenerator
{
public:
	virtual ~INicknameGenerator() = default;
	
	virtual FName Generate() = 0;

protected:
	FString TakeRandom(TArray<FString>& array) const;
};

inline FString INicknameGenerator::TakeRandom(TArray<FString>& array) const
{
	if (array.Num() == 0)
	{
		return "";
	}
	auto const index = FMath::RandRange(0, array.Num() - 1);
	auto const taken = array[index];
	array.RemoveAt(index);
	return taken;
}
