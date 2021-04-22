#pragma once

#include "IAnnouncementProcessor.h"

/// <summary>
/// Frag announcement processor
/// </summary>
class FragAnnouncementProcessor final : public IAnnouncementProcessor
{
public:
	FragAnnouncementProcessor() = default;

	virtual bool TryProcessAnnouncement(
		const IAnnouncement* announcement,
		IAnnouncer* announcer
	) override;
};
