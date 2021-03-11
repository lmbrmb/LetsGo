#include "MatchAnalytics.h"

#include "ExcellentMedalProcessor.h"
#include "FirstBloodMedalProcessor.h"
#include "ImpressiveMedalProcessor.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

MatchAnalytics::MatchAnalytics(AMatchGameMode* matchGameMode)
{
	matchGameMode->AvatarSpawned.AddRaw(this, &MatchAnalytics::OnAvatarSpawned);
	_damageMedalProcessors.Add(new FirstBloodMedalProcessor());
	_damageMedalProcessors.Add(new ExcellentMedalProcessor());
	_damageMedalProcessors.Add(new ImpressiveMedalProcessor());
	_healthProcessors.Add([this](const UHealthComponent* healthComponent, const float delta) { TryProcessDamage(healthComponent, delta); });
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
	for (auto healthProcessor : _healthProcessors)
	{
		healthProcessor(healthComponent, delta);
	}
}

void MatchAnalytics::TryProcessDamage(const UHealthComponent* healthComponent, const float delta)
{
	if (delta >= 0)
	{
		return;
	}

	auto const damagedPlayerActor = healthComponent->GetOwner();
	auto const damagedPlayerAvatar = Cast<AAvatar>(damagedPlayerActor);
	AssertIsNotNull(damagedPlayerAvatar);
	auto const lastDamage = healthComponent->GetLastDamage();
	auto const time = _world->TimeSeconds;
	auto const damageEvent = DamageEvent(
		time,
		lastDamage.GetInstigatorId(),
		lastDamage.GetWeaponId(),
		damagedPlayerAvatar->GetPlayerId(),
		healthComponent->GetCurrentValue()
	);
	ProcessDamageEvent(damageEvent);
}

void MatchAnalytics::ProcessDamageEvent(const DamageEvent& damageEvent)
{
	Medal medal;
	for(auto damageMedalProcessor : _damageMedalProcessors)
	{
		auto const isMedalAchieved = damageMedalProcessor->ProcessDamageEvent(damageEvent, medal);
		if (isMedalAchieved)
		{
			AssertIsNotEqual((int)medal.MedalType, (int)FMedalType::None);
			MedalAchieved.Broadcast(medal);
		}
	}
}
