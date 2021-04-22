#pragma once

#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncer.h"

class IAnnouncementProcessor
{
public:
	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) = 0;

protected:
	virtual ~IAnnouncementProcessor() = default;
};

