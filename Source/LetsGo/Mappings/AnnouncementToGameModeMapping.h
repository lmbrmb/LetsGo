#pragma once

#include "MappingComponent.h"
#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"

#include "AnnouncementToGameModeMapping.generated.h"

/// <summary>
/// Announcement to game mode mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToGameModeMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UAnnouncementManagerComponent* _announcementManagerComponent;
	
	void OnAnnouncementManagerComponentInitialized();

	static void SubscribeMatchStateEvents(
		AMatchGameMode* matchGameMode,
		IAnnouncementManager* announcementManager
	);
};
