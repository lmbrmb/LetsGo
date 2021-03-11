#include "Medal.h"

Medal::Medal(): PlayerId(0), MedalType(FMedalType::None)
{
}

Medal::Medal(const int32 playerId, const FMedalType medalType)
	: PlayerId(playerId),
	MedalType(medalType)
{
}
