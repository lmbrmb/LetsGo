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
	FString TakeRandom(TArray<FString>& arr) const;
};

inline FString INicknameGenerator::TakeRandom(TArray<FString>& arr) const
{
	if (arr.Num() == 0)
	{
		return "";
	}
	auto const index = FMath::RandRange(0, arr.Num() - 1);
	auto const taken = arr[index];
	arr.RemoveAt(index);
	return taken;
}
