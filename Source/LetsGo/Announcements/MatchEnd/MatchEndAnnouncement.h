#pragma once

#include "LetsGo/Announcements/IAnnouncement.h"

/// <summary>
/// Match end announcement
/// </summary>
class MatchEndAnnouncement final : public IAnnouncement
{
public:
	explicit MatchEndAnnouncement(const int localPlayerPlace);

	int GetLocalPlayerPlace() const;

private:
	int _localPlayerPlace;
};
