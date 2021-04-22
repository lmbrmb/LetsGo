#pragma once

DECLARE_EVENT_OneParam(IAnnouncementManager, EAnnouncementRequest, const IAnnouncement* announcement);

DECLARE_EVENT(IAnnouncementManager, EAllAnnouncementsDone);

class IAnnouncementManager
{
public:
	EAnnouncementRequest AnnouncementRequest;

	EAllAnnouncementsDone AllAnnouncementsDone;

protected:
	virtual ~IAnnouncementManager() = default;
};
