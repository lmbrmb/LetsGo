#include "FragAnnouncementFactory.h"

#include "FragAnnouncement.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
IAnnouncement* FragAnnouncementFactory::Create(
	const PlayerId& localPlayerId,
	const PlayerId& instigatorPlayerId,
	const PlayerId& fraggedPlayerId,
	AMatchGameMode* matchGameMode
)
{
	auto const isLocalPlayerInstigator = localPlayerId == instigatorPlayerId;
	auto const isLocalPlayerFragged = localPlayerId == fraggedPlayerId;
	auto const isRelevantToLocalPlayer = isLocalPlayerInstigator || isLocalPlayerFragged;

	if (!isRelevantToLocalPlayer)
	{
		return nullptr;
	}
	
	auto const instigatorPlayerNickname = matchGameMode->GetPlayerNickname(instigatorPlayerId);
	auto const fraggedPlayerNickname = matchGameMode->GetPlayerNickname(fraggedPlayerId);
	auto const instigatorPlayerPlace = matchGameMode->GetPlayerPlace(instigatorPlayerId);
	auto const fraggedPlayerPlace = matchGameMode->GetPlayerPlace(fraggedPlayerId);

	auto const fragAnnouncement = new FragAnnouncement(
		instigatorPlayerNickname,
		fraggedPlayerNickname,
		isLocalPlayerInstigator,
		isLocalPlayerFragged,
		instigatorPlayerPlace,
		fraggedPlayerPlace
	);
	
	return fragAnnouncement;
}
