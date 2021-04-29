#include "AvatarSfxComponent.h"

#include "AvatarSfxFactory.h"
#include "LetsGo/GameModes/ProjectGameModeBase.h"
#include "LetsGo/Utils/AssertUtils.h"

const float UAvatarSfxComponent::MIN_AIR_TIME = 0.05f;

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
	_groundStepSounds = avatarSfxFactory->GetGroundStepSoundsBySkin(skinId);
	_waterStepSounds = avatarSfxFactory->GetWaterStepSounds();
	_deathSounds = avatarSfxFactory->GetDeathSoundsBySkin(skinId);
	_painSounds = avatarSfxFactory->GetPainSounds(skinId);
}

void UAvatarSfxComponent::OnStep(const MovementSpeedState movementSpeedState)
{
	// Walk is silent
	if(movementSpeedState == MovementSpeedState::Walk)
	{
		return;
	}

	auto const stepSound = GetStepSound();

	if(!stepSound)
	{
		return;
	}

	BpPlaySound(stepSound);
}

void UAvatarSfxComponent::OnJump()
{
	BpPlaySound(_jumpSound);
}

void UAvatarSfxComponent::OnLand(const float airTime)
{
	if(airTime < MIN_AIR_TIME)
	{
		return;
	}
	
	BpPlaySound(_landSound);
}

void UAvatarSfxComponent::OnEnvironmentChanged(const FEnvironment environment)
{
	_environment = environment;
}

void UAvatarSfxComponent::OnHealthChanged(UHealthComponent* healthComponent, const float delta)
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

USoundBase* UAvatarSfxComponent::GetStepSound()
{
	auto const& soundArray = _environment == FEnvironment::Water ? _waterStepSounds : _groundStepSounds;
	auto const stepSoundsCount = soundArray.Num();
	AssertIsGreaterOrEqual(stepSoundsCount, 1, nullptr);

	auto nextStepSoundIndex = _stepSoundIndex + 1;

	if (nextStepSoundIndex >= stepSoundsCount)
	{
		nextStepSoundIndex = 0;
	}

	_stepSoundIndex = nextStepSoundIndex;

	return soundArray[_stepSoundIndex];
}
