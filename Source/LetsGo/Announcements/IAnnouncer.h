#pragma once

#include "Lead/LeadState.h"
#include "LetsGo/Analytics/MedalType.h"

/// <summary>
/// Announcer contract
/// </summary>
class IAnnouncer
{
public:
	virtual void OnAnnouncementRequest(const IAnnouncement* announcement) = 0;
	
	virtual void OnAllAnnouncementsDone() = 0;
	
	virtual void AnnounceMatchWarmUp() = 0;

	virtual void AnnounceMatchStart() = 0;

	virtual void AnnounceMatchEnd(const int localPlayerPlace) = 0;

	virtual void AnnounceFrag(
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname,
		const bool isLocalPlayerInstigator,
		const bool isLocalPlayerFragged,
		const int instigatorPlayerPlace,
		const int fraggedPlayerPlace
	) = 0;

	virtual void AnnounceMedal(const FMedalType medalType) = 0;

	virtual void AnnounceLead(const FLeadState leadState) = 0;

protected:
	virtual ~IAnnouncer() = default;
};
