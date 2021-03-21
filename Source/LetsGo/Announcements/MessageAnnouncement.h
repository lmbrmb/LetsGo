#pragma once

#include "Announcement.h"

class MessageAnnouncement final : public Announcement
{
public:
	MessageAnnouncement(const FString& message, const bool isRelevantToLocalPlayer);

	bool GetIsRelevantToLocalPlayer() const;
	
	const FString& GetMessage() const
	{
		return _message;
	}

private:
	FString _message;

	bool _isRelevantToLocalPlayer;
};
