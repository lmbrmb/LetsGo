#pragma once

#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncementFactory.h"
#include "LetsGo/Data/PlayerId.h"

class AMatchGameMode;

/// <summary>
/// Match end announcement factory
/// </summary>
class MatchEndAnnouncementFactory final : public IAnnouncementFactory
{
public:
	IAnnouncement* Create(
		const PlayerId& localPlayerId,
		AMatchGameMode* matchGameMode
	);
};
