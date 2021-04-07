#pragma once

#include "Components/ActorComponent.h"
#include "LetsGo/Data/SkinId.h"

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

	void OnStep();

	void OnJump();

	void OnLand();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnStep();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnJump();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnLand();

private:
	SkinId _skinId;
};
