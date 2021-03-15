#pragma once

#include "Animation/AnimInstance.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Movement/MovementComponentBase.h"

#include "AvatarAnimInstance.generated.h"

// Remarks (based on UnrealEngine v4.26.1 usage experience):
// 1. In blueprint: when binding custom event to dynamic multicast delegate
// event signature sometimes can't be determined and blueprint is corrupted.
// (can happen after recompilation of modified relative code)
// 2. When caching components in BlueprintBeginPlay,
// it works pretty fine in editor but results in
// "soft object reference is not found" in build

/// <summary>
/// Avatar anim instance.
/// Caches avatar components and exposes component events to blueprint
/// Component values are blueprint's responsibility, should not be cached here
/// </summary>
UCLASS()
class LETSGO_API UAvatarAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();

	UFUNCTION(BlueprintCallable)
	UMovementComponentBase* GetMovementComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAvatarDied();
	
protected:
	virtual void NativeBeginPlay() override;
	
private:
	UHealthComponent* _healthComponent = nullptr;
	
	UMovementComponentBase* _movementComponentBase = nullptr;

	void OnDied(const UHealthComponent*, float delta);
};
