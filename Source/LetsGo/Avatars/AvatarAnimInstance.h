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
// 3. If there's a call to component in blueprint,
// component class modification results in
// "HOTRELOADED" component class error in blueprint.
// Have to restart editor, recompile again,
// re-establish all component-related nodes.

/// <summary>
/// Avatar anim instance.
/// [Decorator]
/// Caches avatar components and exposes component methods to blueprint
/// Component values are blueprint's responsibility, should not be cached here
/// </summary>
UCLASS()
class LETSGO_API UAvatarAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAvatarDied();

	UFUNCTION(BlueprintCallable)
	float GetMovementSpeedValue() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsInAirValue() const;

	virtual void NativeBeginPlay() override;

private:
	UHealthComponent* _healthComponent = nullptr;

	UMovementComponentBase* _movementComponentBase = nullptr;

	void OnDied(UHealthComponent* healthComponent, const float delta);
};
