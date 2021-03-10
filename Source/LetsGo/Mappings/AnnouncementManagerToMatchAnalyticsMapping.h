#pragma once

#include "MappingComponent.h"

#include "AnnouncementManagerToMatchAnalyticsMapping.generated.h"

///<summary>
///Announcement manager to match analytics mapping
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementManagerToMatchAnalyticsMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;
};
