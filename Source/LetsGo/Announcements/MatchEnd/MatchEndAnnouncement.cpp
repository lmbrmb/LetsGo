#include "MatchEndAnnouncement.h"

MatchEndAnnouncement::MatchEndAnnouncement(const int localPlayerPlace)
	: _localPlayerPlace(localPlayerPlace)
{
}

int MatchEndAnnouncement::GetLocalPlayerPlace() const
{
	return _localPlayerPlace;
}
