#pragma once

#include "MatchGameMode.h"
#include "LetsGo/Avatars/AvatarDataFactory.h"

#include "DeathmatchGameMode.generated.h"

DECLARE_EVENT_OneParam(ADeathmatchGameMode, ELeadTied, PlayerId playerId);

DECLARE_EVENT_OneParam(ADeathmatchGameMode, ELeadTaken, PlayerId playerId);

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
	virtual void PopulateAvatarsData() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void OnFragsCountChanged() override;

	virtual bool IsLocalPlayerWonMatch() override;

	virtual int GetPlayerPlace(const PlayerId& playerId) const override;

private:
	const FName LOCAL_PLAYER_NAME = "%UserName%";

	PlayerId _localPlayerId;

	PlayerId _winnerPlayerId;

	AvatarDataFactory* _avatarDataFactory;

	TMap<int, int> _playerPlaces;

	void UpdatePlayerPlaces();
};
