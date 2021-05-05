#pragma once

#include "IAnnouncementManager.h"
#include "Frag/FragAnnouncementFactory.h"
#include "Lead/LeadAnnouncementFactory.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "MatchEnd/MatchEndAnnouncementFactory.h"
#include "MatchStart/MatchStartAnnouncementFactory.h"
#include "MatchWarmUp/MatchWarmUpAnnouncementFactory.h"
#include "Medal/MedalAnnouncementFactory.h"

class AnnouncementManagerFactory final
{
public:
	AnnouncementManagerFactory();

	void SetTimings(
		const float firstAnnouncementDelay,
		const float announcementDuration
	);
	
	IAnnouncementManager* Create(
		AMatchGameMode* matchGameMode,
		const PlayerId& playerId
	) const;

private:
	FragAnnouncementFactory* _fragAnnouncementFactory = nullptr;

	MedalAnnouncementFactory* _medalAnnouncementFactory = nullptr;

	MatchWarmUpAnnouncementFactory* _matchWarmUpAnnouncementFactory = nullptr;

	MatchStartAnnouncementFactory* _matchStartAnnouncementFactory = nullptr;

	MatchEndAnnouncementFactory* _matchEndAnnouncementFactory = nullptr;

	LeadAnnouncementFactory* _leadAnnouncementFactory = nullptr;

	float _firstAnnouncementDelay = 0.0f;

	float _announcementDuration = 0.0f;
};
