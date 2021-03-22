#include "FragAnnouncement.h"

FragAnnouncement::FragAnnouncement(
	const FName& instigatorPlayerNickname, 
	const FName& fraggedPlayerNickname,
	const bool isLocalPlayerInstigator, 
	const bool isLocalPlayerFragged
) : Announcement(),
_instigatorPlayerNickname(instigatorPlayerNickname),
_fraggedPlayerNickname(fraggedPlayerNickname),
_isLocalPlayerInstigator(isLocalPlayerInstigator),
_isLocalPlayerFragged(isLocalPlayerFragged)
{
}

FName FragAnnouncement::GetInstigatorPlayerNickname() const
{
	return _instigatorPlayerNickname;
}

FName FragAnnouncement::GetFraggedPlayerNickname() const
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
