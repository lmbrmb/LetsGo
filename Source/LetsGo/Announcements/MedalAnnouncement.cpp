#include "MedalAnnouncement.h"

#include "LetsGo/Analytics/MedalType.h"

MedalAnnouncement::MedalAnnouncement(const FMedalType medalType) :
	Announcement(),
	_medalType(medalType)
{
}

FMedalType MedalAnnouncement::GetMedalType() const
{
	return _medalType;
}
