#pragma once

#include "LetsGo/Analytics/MedalType.h"
#include "LetsGo/Announcements/IAnnouncement.h"

/// <summary>
/// Medal announcement
/// </summary>
class MedalAnnouncement final : public IAnnouncement
{
public:
	explicit MedalAnnouncement(const FMedalType medalType);

	FMedalType GetMedalType() const;

private:
	FMedalType _medalType;
};
