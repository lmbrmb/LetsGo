#include "NicknameGeneratorFactory.h"

#include "Q3NicknameGenerator.h"

const float PREDEFINED_NICKNAME_PROBABILITY = 0.2f;

NicknameGeneratorFactory::NicknameGeneratorFactory()
{
}

// ReSharper disable once CppMemberFunctionMayBeStatic
INicknameGenerator* NicknameGeneratorFactory::Create()
{
	return new Q3NicknameGenerator();
}
