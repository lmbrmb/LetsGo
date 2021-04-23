#pragma once

#include "MappingComponent.h"
#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/Hud/PlayerHud.h"

#include "AnnouncementToHudMapping.generated.h"

/// <summary>
/// Announcement to HUD mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementToHudMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	APlayerHud* _playerHud = nullptr;

	UAnnouncementManagerComponent* _announcementManagerComponent = nullptr;

	bool _isPlayerHudInitialized = false;

	bool _isAnnouncementManagerComponentInitialized = false;

	void OnPlayerHudInitialized();

	void OnAnnouncementManagerComponentInitialized();

	void OnPartialInitialization();

	void Bind();
};
