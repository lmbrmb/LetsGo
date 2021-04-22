#pragma once

#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncementFactory.h"

class MatchWarmUpAnnouncementFactory final : public IAnnouncementFactory
{
public:
	IAnnouncement* Create();
};
