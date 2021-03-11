#pragma once

#include "MedalType.h"

class Medal
{
public:
	Medal();
	
	explicit Medal(const int32 playerId, const FMedalType medalType);

	int32 PlayerId;

	FMedalType MedalType;
};
