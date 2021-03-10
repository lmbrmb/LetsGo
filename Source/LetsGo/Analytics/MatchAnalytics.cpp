#include "MatchAnalytics.h"

#include "FirstBloodMatchEventProcessor.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

MatchAnalytics::MatchAnalytics(AMatchGameMode* matchGameMode)
{
	matchGameMode->AvatarSpawned.AddRaw(this, &MatchAnalytics::OnAvatarSpawned);
	_matchEventProcessors.Add(new FirstBloodMatchEventProcessor());
	_world = matchGameMode->GetWorld();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void MatchAnalytics::OnAvatarSpawned(const AAvatar* avatar)
{
	auto const healthComponent = avatar->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent)
	healthComponent->HealthChanged.AddRaw(this, &MatchAnalytics::OnAvatarHealthChanged);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void MatchAnalytics::OnAvatarHealthChanged(const UHealthComponent* healthComponent, const float delta)
{
	auto const damagedPlayerActor = healthComponent->GetOwner();
	auto const damagedPlayerAvatar = Cast<AAvatar>(damagedPlayerActor);
	AssertIsNotNull(damagedPlayerAvatar);
	auto const lastDamage = healthComponent->GetLastDamage();
	auto const time = _world->TimeSeconds;
	auto const matchEvent = MatchEvent(
		time,
		lastDamage.GetInstigatorId(),
		damagedPlayerAvatar->GetPlayerId(),
		healthComponent->GetCurrentValue()
	);
	ProcessMatchEvent(matchEvent);
}

void MatchAnalytics::ProcessMatchEvent(const MatchEvent& matchEvent)
{
	for(auto matchEventProcessor : _matchEventProcessors )
	{
		auto const highlight = matchEventProcessor->ProcessMatchEvent(matchEvent);
		if (highlight != FMatchHighlight::None)
		{
			auto const playerId = matchEvent.InstigatorPlayerId;
			MatchHighlight.Broadcast(playerId, highlight);
		}
	}
}
