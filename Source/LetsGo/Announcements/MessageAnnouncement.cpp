#include "MessageAnnouncement.h"

MessageAnnouncement::MessageAnnouncement(const FString& message, const bool isRelevantToLocalPlayer) :
	Announcement(),
	_message(message),
	_isRelevantToLocalPlayer(isRelevantToLocalPlayer)
{
}

bool MessageAnnouncement::GetIsRelevantToLocalPlayer() const
{
	return _isRelevantToLocalPlayer;
}