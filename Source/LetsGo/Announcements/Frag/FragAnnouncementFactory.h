#pragma once

#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncementFactory.h"
#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/GameModes/MatchGameMode.h"

class FragAnnouncementFactory final : public IAnnouncementFactory
{
public:
	IAnnouncement* Create(
		const PlayerId& localPlayerId,
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId,
		AMatchGameMode* matchGameMode
	);
};
