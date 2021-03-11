#pragma once

#include "MappingComponent.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "AvatarToWeaponManagerMapping.generated.h"

///<summary>
/// Avatar to weapon manager mapping
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAvatarToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	 void OnAvatarInitialized(const AAvatar* avatar);

	 UWeaponManagerComponent* _weaponManagerComponent;
};
