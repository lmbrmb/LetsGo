#include "AvatarSfxComponent.h"

#include "AvatarSfxFactory.h"
#include "LetsGo/GameModes/ProjectGameModeBase.h"
#include "LetsGo/Utils/AssertUtils.h"

UAvatarSfxComponent::UAvatarSfxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAvatarSfxComponent::SetSkinId(const SkinId& skinId)
{
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const projectGameModeBase = Cast<AProjectGameModeBase>(authGameMode);
	AssertIsNotNull(projectGameModeBase);
	auto const diContainer = projectGameModeBase->GetDiContainer();

	auto const avatarSfxFactoryRef = diContainer->GetInstance<AvatarSfxFactory>();
	auto const avatarSfxFactory = &avatarSfxFactoryRef.Get();

	_jumpSound = avatarSfxFactory->GetJumpSound(skinId);
	_landSound = avatarSfxFactory->GetLandSound(skinId);
	_stepSounds = avatarSfxFactory->GetStepSounds(skinId);
	_deathSounds = avatarSfxFactory->GetDeathSounds(skinId);
	_painSounds = avatarSfxFactory->GetPainSounds(skinId);
}

void UAvatarSfxComponent::OnStep()
{
	auto const stepSoundsCount = _stepSounds.Num();
	AssertIsGreaterOrEqual(stepSoundsCount, 1);
	
	auto nextStepSoundIndex = _stepSoundIndex + 1;
	
	if (nextStepSoundIndex >= stepSoundsCount)
	{
		nextStepSoundIndex = 0;
	}

	_stepSoundIndex = nextStepSoundIndex;
	auto const stepSound = _stepSounds[_stepSoundIndex];

	BpPlaySound(stepSound);
}

void UAvatarSfxComponent::OnJump()
{
	BpPlaySound(_jumpSound);
}

void UAvatarSfxComponent::OnLand()
{
	BpPlaySound(_landSound);
}

void UAvatarSfxComponent::OnHealthChanged(const UHealthComponent* healthComponent, const float delta)
{
	auto const currentHealth = healthComponent->GetCurrentValue();
	auto const previousHealth = currentHealth - delta;
	
	if(healthComponent->IsDead())
	{
		auto const deathSoundsCount = _deathSounds.Num();
		AssertIsGreaterOrEqual(deathSoundsCount, 1);
		auto const randomDeathSoundIndex = FMath::RandRange(0, deathSoundsCount - 1);
		auto const randomDeathSound = _deathSounds[randomDeathSoundIndex];
		BpPlaySound(randomDeathSound);
		return;
	}

	for (auto const painSoundRecord : _painSounds)
	{
		auto const targetHealth = painSoundRecord.Key;
		
		if (currentHealth < targetHealth && previousHealth >= targetHealth)
		{
			auto const painSound = painSoundRecord.Value;
			BpPlaySound(painSound);
			return;
		}
	}
}
