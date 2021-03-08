#pragma once

#include "MappingComponent.h"

#include "LetsGo/Avatars/AvatarData.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "AvatarToWeaponManagerMapping.generated.h"

///<summary>
/// Avatar to weapon manager mapping
///</summary>
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LETSGO_API UAvatarToWeaponManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

	virtual bool ShouldDestroyAfterMapping() const override;

private:
	 void OnAvatarDataSet(const AvatarData* avatarData);

	 UWeaponManagerComponent* _weaponManagerComponent;
};
