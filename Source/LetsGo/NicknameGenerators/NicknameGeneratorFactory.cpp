#include "NicknameGeneratorFactory.h"

#include "NicknameGenerator.h"

const float PREDEFINED_NICKNAME_PROBABILITY = 0.2f;

NicknameGeneratorFactory::NicknameGeneratorFactory()
{
}

// ReSharper disable once CppMemberFunctionMayBeStatic
INicknameGenerator* NicknameGeneratorFactory::Create()
{
	return new NicknameGenerator(PREDEFINED_NICKNAME_PROBABILITY);
}
