#pragma once

#include "IAnnouncementManager.h"
#include "Frag/FragAnnouncementFactory.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "MatchEnd/MatchEndAnnouncementFactory.h"
#include "MatchStart/MatchStartAnnouncementFactory.h"
#include "MatchWarmUp/MatchWarmUpAnnouncementFactory.h"
#include "Medal/MedalAnnouncementFactory.h"

class AnnouncementManagerFactory final
{
public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	AnnouncementManagerFactory(AMatchGameMode* matchGameMode);

	void SetTimings(
		const float matchWarmUpAnnouncementDuration,
		const float matchStartAnnouncementDuration,
		const float matchEndAnnouncementDuration,
		const float firstPlayerAnnouncementDelay,
		const float playerAnnouncementDuration
	);
	
	IAnnouncementManager* Create() const;

private:
	AMatchGameMode* _matchGameMode;

	FragAnnouncementFactory* _fragAnnouncementFactory = nullptr;

	MedalAnnouncementFactory* _medalAnnouncementFactory = nullptr;

	MatchWarmUpAnnouncementFactory* _matchWarmUpAnnouncementFactory = nullptr;

	MatchStartAnnouncementFactory* _matchStartAnnouncementFactory = nullptr;

	MatchEndAnnouncementFactory* _matchEndAnnouncementFactory = nullptr;

	float _matchWarmUpAnnouncementDuration = 0.0f;

	float _matchStartAnnouncementDuration = 0.0f;

	float _matchEndAnnouncementDuration = 0.0f;

	float _firstPlayerAnnouncementDelay = 0.0f;

	float _playerAnnouncementDuration = 0.0f;
};
