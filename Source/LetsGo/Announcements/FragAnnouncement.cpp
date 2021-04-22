#include "FragAnnouncement.h"

FragAnnouncement::FragAnnouncement(
	const FName& instigatorPlayerNickname,
	const FName& fraggedPlayerNickname,
	const bool isLocalPlayerInstigator,
	const bool isLocalPlayerFragged,
	const int instigatorPlayerPlace,
	const int fraggedPlayerPlace
) : Announcement(),
_instigatorPlayerNickname(instigatorPlayerNickname),
_fraggedPlayerNickname(fraggedPlayerNickname),
_isLocalPlayerInstigator(isLocalPlayerInstigator),
_isLocalPlayerFragged(isLocalPlayerFragged),
_instigatorPlayerPlace(instigatorPlayerPlace),
_fraggedPlayerPlace(fraggedPlayerPlace)
{
}

const FName& FragAnnouncement::GetInstigatorPlayerNickname() const
{
	return _instigatorPlayerNickname;
}

const FName& FragAnnouncement::GetFraggedPlayerNickname() const
{
	return _fraggedPlayerNickname;
}

bool FragAnnouncement::GetIsLocalPlayerInstigator() const
{
	return _isLocalPlayerInstigator;
}

bool FragAnnouncement::GetIsLocalPlayerFragged() const
{
	return _isLocalPlayerFragged;
}

int FragAnnouncement::GetInstigatorPlayerPlace() const
{
	return _instigatorPlayerPlace;
}

int FragAnnouncement::GetFraggedPlayerPlace() const
{
	return _fraggedPlayerPlace;
}
