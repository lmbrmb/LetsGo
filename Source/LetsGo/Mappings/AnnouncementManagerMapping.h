#pragma once

#include "MappingComponent.h"
#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Hud/PlayerHud.h"


#include "AnnouncementManagerMapping.generated.h"

///<summary>
///Announcement manager mapping. Map to Analytics, GameMode and HUD
///</summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UAnnouncementManagerMapping final : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	UAnnouncementManagerComponent* _announcementManagerComponent = nullptr;

	AMatchGameMode* _matchGameMode = nullptr;

	APlayerHud* _playerHud = nullptr;

	bool _isAnnouncementManagerComponentInitialized = false;

	bool _isPlayerHudInitialized = false;

	void OnAnnouncementManagerComponentInitialized();

	void OnPlayerHudInitialized();

	void OnPartialInitialization() const;

	void Bind() const;

	void UnBind() const;

	void OnMatchEnd() const;

	void OnExit() const;
};
