#pragma once

#include "Components/AudioComponent.h"
#include "Components/ActorComponent.h"
#include "LetsGo/Data/SkinId.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "Sound/SoundBase.h"
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

	void SetAudioComponent(UAudioComponent* audioComponent);

	void OnStep();

	void OnJump() const;

	void OnLand() const;

	void OnHealthChanged(const UHealthComponent* healthComponent, const float delta);

private:
	UPROPERTY()
	TMap<float, USoundBase*> _painSounds;

	UPROPERTY()
	TArray<USoundBase*> _deathSounds;

	UPROPERTY()
	USoundBase* _jumpSound;

	UPROPERTY()
	USoundBase* _landSound;

	UPROPERTY()
	TArray<USoundBase*> _stepSounds;

	UPROPERTY()
	UAudioComponent* _audioComponent;

	int _stepSoundIndex = -1;

	void PlaySound(USoundBase* sound) const;
};
