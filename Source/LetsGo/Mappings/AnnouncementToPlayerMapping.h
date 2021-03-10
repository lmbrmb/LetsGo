#pragma once

#include "MappingComponent.h"
#include "AnnouncementToPlayerMapping.generated.h"

///<summary>
///Announcement manager to player controller mapping
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToPlayerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
