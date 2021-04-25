#pragma once

#include "IAnnouncementProcessor.h"

/// <summary>
/// Lead announcement processor
/// </summary>
class LeadAnnouncementProcessor final : public IAnnouncementProcessor
{
public:
	LeadAnnouncementProcessor() = default;

	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) override;
};
