#pragma once

#include "IAnnouncementProcessor.h"

/// <summary>
/// Match warm up announcement processor
/// </summary>
class MatchWarmUpAnnouncementProcessor final : public IAnnouncementProcessor
{
public:
	MatchWarmUpAnnouncementProcessor() = default;

	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) override;
};
