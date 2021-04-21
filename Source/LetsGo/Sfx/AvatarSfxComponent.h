#pragma once

#include "Components/ActorComponent.h"
#include "LetsGo/Data/SkinId.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "Sound/SoundBase.h"
#include "LetsGo/Movement/MovementSpeedState.h"

#include "AvatarSfxComponent.generated.h"

/// <summary>
/// Avatar SFX component
/// </summary>
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UAvatarSfxComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAvatarSfxComponent();

	void SetSkinId(const SkinId& skinId);

	void OnStep(const MovementSpeedState movementSpeedState);

	void OnJump();

	void OnLand(const float airTime);

	void OnHealthChanged(UHealthComponent* healthComponent, const float delta);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpPlaySound(const USoundBase* sound);

private:
	static const float MIN_AIR_TIME;

	TMap<float, USoundBase*> _painSounds;

	TArray<USoundBase*> _deathSounds;

	USoundBase* _jumpSound = nullptr;

	USoundBase* _landSound = nullptr;

	TArray<USoundBase*> _stepSounds;

	int _stepSoundIndex = -1;
};
