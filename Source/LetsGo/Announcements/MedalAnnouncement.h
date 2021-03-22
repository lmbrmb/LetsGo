#pragma once

#include "Announcement.h"
#include "LetsGo/Analytics/MedalType.h"

/// <summary>
/// Medal announcement
/// </summary>
class MedalAnnouncement final : public Announcement
{
public:
	explicit MedalAnnouncement(const FMedalType medalType);

	FMedalType GetMedalType() const;

private:
	FMedalType _medalType;
};
