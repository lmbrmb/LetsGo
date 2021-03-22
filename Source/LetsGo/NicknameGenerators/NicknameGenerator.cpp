#include "NicknameGenerator.h"

#include "Kismet/KismetStringLibrary.h"
#include "LetsGo/Utils/FStringUtils.h"
#include "LetsGo/Utils/MathUtils.h"

NicknameGenerator::NicknameGenerator(const float predefinedNicknameProbability):
	_predefinedNicknameProbability(predefinedNicknameProbability)
{
	//TODO: import from csv
	_predefinedNicknames.Add("private static void");
	_predefinedNicknames.Add("TimeTravell3r");
	_predefinedNicknames.Add("MegaMan");

	_nicknamePrefixes.Add("Mighty");
	_nicknamePrefixes.Add("Divine");
	_nicknamePrefixes.Add("Eternal");

	_nicknameNouns.Add("Orb");
	_nicknameNouns.Add("N00b");
	_nicknameNouns.Add("Cl3rIc");
	_nicknameNouns.Add("Ninja");
	_nicknameNouns.Add("Star");

	_nicknamePostfixes.Add("Crusher");
	_nicknamePostfixes.Add("9001");
	_nicknamePostfixes.Add(" 0_o");
}

FName NicknameGenerator::Generate()
{
	FString name;

	if(MathUtils::TestProbability(_predefinedNicknameProbability))
	{
		// Predefined
		name = TakeRandom(_predefinedNicknames);
		if (!UKismetStringLibrary::IsEmpty(name))
		{
			return FName(name);
		}
	}

	// Composite
	name = TakeRandom(_nicknameNouns);
	
	if (!UKismetStringLibrary::IsEmpty(name))
	{
		FString part;
		
		if (FMath::RandBool())
		{
			part = TakeRandom(_nicknamePrefixes);
			name = part + name;
		}

		if (FMath::RandBool())
		{
			part = TakeRandom(_nicknamePostfixes);
			name += part;
		}
	}

	if (UKismetStringLibrary::IsEmpty(name) || _generatedNames.Contains(name))
	{
		//Unique 4 digit number
		auto const randomNumber = FMath::RandRange(100, 999);
		auto const generatedNamesCount = _generatedNames.Num();
		name = FStringUtils::ToString(randomNumber) + FStringUtils::ToString(generatedNamesCount);
	}

	_generatedNames.Add(name);
	return FName(name);
}
