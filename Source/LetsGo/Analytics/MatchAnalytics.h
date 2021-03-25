#pragma once

#include "IDamageMedalProcessor.h"
#include "IHitMedalProcessor.h"
#include "DamageEvent.h"
#include "Medal.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

class AMatchGameMode;

DECLARE_EVENT_OneParam(MatchAnalytics, EMedalAchieved, const Medal& medal);

/// <summary>
/// Analyzes match events. Checks and notifies if player earned medal
/// </summary>
class MatchAnalytics
{
public:
	explicit MatchAnalytics(AMatchGameMode* matchGameMode);

	EMedalAchieved MedalAchieved;

private:
	void OnAvatarSpawned(const AAvatar* avatar);

	void OnAvatarHealthChanged(const UHealthComponent* healthComponent, const float delta);

	void OnShotPerformed(const PlayerId& instigatorId, const WeaponType& instigatorWeaponType, const bool isHittedPlayer);

	void TryProcessDamage(const UHealthComponent* healthComponent, const float delta);
	
	void ProcessDamageEvent(const DamageEvent& damageEvent);

	TArray<TFunction<void(const UHealthComponent*, const float)>> _healthProcessors;
	
	TArray<IDamageMedalProcessor*> _damageMedalProcessors;

	TArray<IHitMedalProcessor*> _hitMedalProcessors;

	AMatchGameMode* _matchGameMode;
	
	UWorld* _world;
};
