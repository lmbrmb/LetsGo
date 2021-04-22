#pragma once

#include "IAnnouncementProcessor.h"

/// <summary>
/// Match end announcement processor
/// </summary>
class MatchEndAnnouncementProcessor final : public IAnnouncementProcessor
{
public:
	MatchEndAnnouncementProcessor() = default;

	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) override;
};

