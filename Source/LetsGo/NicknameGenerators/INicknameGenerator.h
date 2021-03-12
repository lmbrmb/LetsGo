#pragma once 

/// <summary>
/// [Interface] Nickname generator contract
/// </summary>
class INicknameGenerator
{
public:
	virtual ~INicknameGenerator() = default;
	
	virtual FName Generate() = 0;
};