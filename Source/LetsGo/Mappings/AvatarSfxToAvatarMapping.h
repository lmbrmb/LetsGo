#pragma once

#include "MappingComponent.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/Sfx/AvatarSfxComponent.h"

#include "AvatarSfxToAvatarMapping.generated.h"

/// <summary>
/// Avatar SFX to avatar mapping component
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAvatarSfxToAvatarMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UAvatarSfxComponent* _avatarSfxComponent;

	void OnAvatarInitialized(const AAvatar* avatar);
};
