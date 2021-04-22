#pragma once

#include "IAnnouncementProcessor.h"

/// <summary>
/// Match start announcement processor
/// </summary>
class MatchStartAnnouncementProcessor final : public IAnnouncementProcessor
{
public:
	MatchStartAnnouncementProcessor() = default;

	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) override;
};

