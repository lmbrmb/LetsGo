#pragma once

#include "MappingComponent.h"
#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/Hud/PlayerHud.h"

#include "AnnouncementToHudMapping.generated.h"

/// <summary>
/// Announcement to HUD mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToHudMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	void OnPlayerHudInitialized();

	APlayerHud* _playerHud = nullptr;
	
	UAnnouncementManagerComponent* _announcementManagerComponent = nullptr;
};
