#pragma once

#include "Announcement.h"
#include "LetsGo/Analytics/MedalType.h"

class MedalAnnouncement final : public Announcement
{
public:
	explicit MedalAnnouncement(const FMedalType medalType);

	FMedalType GetMedalType() const;

private:
	FMedalType _medalType;
};
