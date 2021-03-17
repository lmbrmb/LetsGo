#include "Medal.h"

Medal::Medal() :
	_medalType(FMedalType::None)
{
}

Medal::Medal(const PlayerId& playerId, const FMedalType medalType)
	: _playerId(playerId),
	_medalType(medalType)
{
}

const PlayerId& Medal::GetPlayerId() const
{
	return _playerId;
}

FMedalType Medal::GetMedalType() const
{
	return _medalType;
}
