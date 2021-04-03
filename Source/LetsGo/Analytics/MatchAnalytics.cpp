#include "MatchAnalytics.h"

#include "ExcellentMedalProcessor.h"
#include "FirstBloodMedalProcessor.h"
#include "ImpressiveMedalProcessor.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

MatchAnalytics::MatchAnalytics(AMatchGameMode* matchGameMode) :
	_matchGameMode(matchGameMode)
{
	_matchGameMode->AvatarSpawned.AddRaw(this, &MatchAnalytics::OnAvatarSpawned);
	_world = matchGameMode->GetWorld();

	auto const firstBloodMedalProcessor = new FirstBloodMedalProcessor();
	auto const impressiveMedalProcessor = new ImpressiveMedalProcessor(2, WeaponType("Railgun"));
	auto const excellentMedalProcessor = new ExcellentMedalProcessor(4);
	
	_damageMedalProcessors.Add(firstBloodMedalProcessor);
	_damageMedalProcessors.Add(excellentMedalProcessor);
	_damageMedalProcessors.Add(impressiveMedalProcessor);

	_hitMedalProcessors.Add(impressiveMedalProcessor);
	
	_healthProcessors.Add([this](const UHealthComponent* healthComponent, const float delta) { this->TryProcessDamage(healthComponent, delta); });
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void MatchAnalytics::OnAvatarSpawned(const AAvatar* avatar)
{
	auto const healthComponent = avatar->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);
	healthComponent->HealthChanged.AddRaw(this, &MatchAnalytics::OnAvatarHealthChanged);

	auto const weaponManagerComponent = avatar->FindComponentByClass<UWeaponManagerComponent>();
	weaponManagerComponent->ShotPerformed.AddRaw(this, &MatchAnalytics::OnShotPerformed);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void MatchAnalytics::OnAvatarHealthChanged(const UHealthComponent* healthComponent, const float delta)
{
	if(!_matchGameMode->IsMatchInProgress())
	{
		return;
	}
	
	for (auto healthProcessor : _healthProcessors)
	{
		healthProcessor(healthComponent, delta);
	}
}

void MatchAnalytics::OnShotPerformed(const PlayerId& instigatorId, const WeaponType& instigatorWeaponType, const bool isHittedPlayer)
{
	if (!_matchGameMode->IsMatchInProgress())
	{
		return;
	}
	
	Medal medal;
	for (auto hitMedalProcessor : _hitMedalProcessors)
	{
		auto const time = _world->TimeSeconds;
		auto const hitEvent = HitEvent(
			time,
			isHittedPlayer,
			instigatorId,
			instigatorWeaponType
		);
		auto const isMedalAchieved = hitMedalProcessor->ProcessHitEvent(hitEvent, medal);
		if (isMedalAchieved)
		{
			AssertIsNotEqual((int)medal.GetMedalType(), (int)FMedalType::None);
			MedalAchieved.Broadcast(medal);
		}
	};
}

void MatchAnalytics::TryProcessDamage(const UHealthComponent* healthComponent, const float delta)
{
	if (!_matchGameMode->IsMatchInProgress())
	{
		return;
	}
	
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
		lastDamage.GetInstigatorPlayerId(),
		lastDamage.GetInstigatorWeaponType(),
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
			AssertIsNotEqual((int)medal.GetMedalType(), (int)FMedalType::None);
			MedalAchieved.Broadcast(medal);
		}
	}
}
