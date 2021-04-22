#pragma once

#include "MappingComponent.h"
#include "LetsGo/Announcements/AnnouncementManagerComponent.h"

#include "AnnouncementToAnalyticsMapping.generated.h"

///<summary>
///Announcement manager to match analytics mapping
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToAnalyticsMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UAnnouncementManagerComponent* _announcementManagerComponent = nullptr;

	void OnAnnouncementManagerComponentInitialized();
};
