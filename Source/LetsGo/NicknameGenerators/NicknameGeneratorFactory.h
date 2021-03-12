#pragma once 

#include "INicknameGenerator.h"

/// <summary>
/// Nickname generator factory
/// </summary>
class NicknameGeneratorFactory final
{
public:
	NicknameGeneratorFactory();
	
	INicknameGenerator* Create();
};

Expose_TNameOf(NicknameGeneratorFactory)