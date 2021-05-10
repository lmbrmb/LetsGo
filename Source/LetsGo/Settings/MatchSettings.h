#pragma once

#include "GameFramework/SaveGame.h"
#include "MatchSettings.generated.h"

///<summary>
///</summary>
UCLASS()
class LETSGO_API UMatchSettings : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	int BotCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	int FragLimit;
};
