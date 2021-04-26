#pragma once

#include "IAnnouncement.h"
#include "LetsGo/Analytics/Medal.h"
#include "LetsGo/Data/PlayerId.h"

DECLARE_EVENT_OneParam(IAnnouncementManager, EAnnouncementRequest, const IAnnouncement* announcement);

DECLARE_EVENT(IAnnouncementManager, EAllAnnouncementsDone);

class IAnnouncementManager
{
public:
	virtual ~IAnnouncementManager() = default;

	virtual void OnMatchWarmUp() = 0;

	virtual void OnMatchStart() = 0;

	virtual void OnMatchEnd() = 0;

	virtual void OnMedalAchieved(const Medal& medal) = 0;

	virtual void OnPlayerFragged(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId
	) = 0;

	virtual void ClearAllAnnouncements() = 0;

	EAnnouncementRequest AnnouncementRequest;

	EAllAnnouncementsDone AllAnnouncementsDone;
};
