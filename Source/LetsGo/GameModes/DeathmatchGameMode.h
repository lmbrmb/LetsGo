#pragma once

#include "MatchGameMode.h"

#include "DeathmatchGameMode.generated.h"

//<summary>
//Deathmatch. Free For All.
//Win conditions:
//a. First player who scores <frag_limit>, wins.
//b. If time runs out of <time_limit>, player with most frags wins.
//</summary>
UCLASS()
class LETSGO_API ADeathmatchGameMode final : public AMatchGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _fragLimit = 10;
};
