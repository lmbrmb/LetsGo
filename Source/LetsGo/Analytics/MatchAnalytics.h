#pragma once

#include "IMatchEventProcessor.h"
#include "MatchHighlight.h"
#include "MatchEvent.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

class AMatchGameMode;

class MatchAnalytics
{
public:
	MatchAnalytics(AMatchGameMode* matchGameMode);
	
	DECLARE_EVENT_TwoParams(
	MatchAnalytics,
		EMatchHighlight,
		const int32 playerId,
		const FMatchHighlight matchHighlight
		);

	EMatchHighlight MatchHighlight;

private:
	void OnAvatarSpawned(const AAvatar* avatar);

	void OnAvatarHealthChanged(const UHealthComponent* healthComponent, const float delta);

	void ProcessMatchEvent(const MatchEvent& matchEvent);

	TArray<IMatchEventProcessor*> _matchEventProcessors;

	UWorld* _world;
};
