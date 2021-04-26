#pragma once

#include "IAnnouncementManager.h"
#include "Frag/FragAnnouncementFactory.h"
#include "Lead/LeadAnnouncementFactory.h"
#include "Lead/LeadState.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "MatchEnd/MatchEndAnnouncementFactory.h"
#include "MatchStart/MatchStartAnnouncementFactory.h"
#include "MatchWarmUp/MatchWarmUpAnnouncementFactory.h"
#include "Medal/MedalAnnouncementFactory.h"

class AnnouncementManager final : public IAnnouncementManager
{
public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	AnnouncementManager(
		AMatchGameMode* matchGameMode,
		const PlayerId& playerId
	);

	void SetFactories(
		FragAnnouncementFactory* fragAnnouncementFactory,
		MedalAnnouncementFactory* medalAnnouncementFactory,
		MatchWarmUpAnnouncementFactory* matchWarmUpAnnouncementFactory,
		MatchStartAnnouncementFactory* matchStartAnnouncementFactory,
		MatchEndAnnouncementFactory* matchEndAnnouncementFactory,
		LeadAnnouncementFactory* leadAnnouncementFactory
	);

	void SetTimings(
		const float matchWarmUpAnnouncementDuration,
		const float matchStartAnnouncementDuration,
		const float matchEndAnnouncementDuration,
		const float firstPlayerAnnouncementDelay,
		const float playerAnnouncementDuration
	);

	virtual void OnMatchWarmUp() override;

	virtual void OnMatchStart() override;

	virtual void OnMatchEnd() override;

	virtual void OnMedalAchieved(const Medal& medal) override;

	virtual void OnPlayerFragged(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId
	) override;

	virtual void ClearAllAnnouncements() override;

private:
	AMatchGameMode* _matchGameMode = nullptr;

	PlayerId _playerId;

	float _matchWarmUpAnnouncementDuration = 0.0f;

	float _matchStartAnnouncementDuration = 0.0f;

	float _matchEndAnnouncementDuration = 0.0f;

	float _firstPlayerAnnouncementDelay = 0.0f;

	float _playerAnnouncementDuration = 0.0f;

	FragAnnouncementFactory* _fragAnnouncementFactory = nullptr;

	MedalAnnouncementFactory* _medalAnnouncementFactory = nullptr;

	MatchWarmUpAnnouncementFactory* _matchWarmUpAnnouncementFactory = nullptr;

	MatchStartAnnouncementFactory* _matchStartAnnouncementFactory = nullptr;

	MatchEndAnnouncementFactory* _matchEndAnnouncementFactory = nullptr;

	LeadAnnouncementFactory* _leadAnnouncementFactory = nullptr;

	TQueue<IAnnouncement*> _announcements;

	static const float UNDEFINED_TIME;

	float _nextAnnouncementTime = UNDEFINED_TIME;

	FTimerHandle _allAnnouncementsDoneTimerHandle;

	FLeadState _leadState = FLeadState::None;

	void CheckLead();

	void CreateFragAnnouncement(
		const PlayerId& instigatorPlayerId,
		const PlayerId& fraggedPlayerId
	);

	void AddAnnouncement(IAnnouncement* announcement);

	void CreateAnnouncementTask(const float delay);

	void CreateAllAnnouncementsDoneTask(const float delay);

	void AnnounceOnTimer();

	void AllAnnouncementsDoneOnTimer() const;
};
