#pragma once

#include "IAnnouncementProcessor.h"

/// <summary>
/// Medal announcement processor
/// </summary>
class MedalAnnouncementProcessor final : public IAnnouncementProcessor
{
public:
	MedalAnnouncementProcessor() = default;

	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) override;
};
