#pragma once

#include "MappingComponent.h"
#include "AnnouncementToAnalyticsMapping.generated.h"

///<summary>
///Announcement manager to match analytics mapping
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToAnalyticsMapping : public UMappingComponent
{
	GENERATED_BODY()

public:
	virtual void Map() override;
};
