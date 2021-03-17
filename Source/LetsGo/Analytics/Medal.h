#pragma once

#include "MedalType.h"

#include "LetsGo/Data/PlayerId.h"

class Medal
{
public:
	Medal();
	
	explicit Medal(const PlayerId& playerId, const FMedalType medalType);

	const PlayerId& GetPlayerId() const;

	FMedalType GetMedalType() const;

private:
	PlayerId _playerId;

	FMedalType _medalType;
};
