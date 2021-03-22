#pragma once

#include "MappingComponent.h"
#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"

#include "AnnouncementToGameModeMapping.generated.h"

/// <summary>
/// Announcement to game mode mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToGameModeMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	static void SubscribeMatchStateEvents(
		AMatchGameMode* matchGameMode,
		UAnnouncementManagerComponent* announcementManagerComponent
	);
};
