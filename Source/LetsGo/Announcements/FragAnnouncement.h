#pragma once

#include "Announcement.h"

/// <summary>
/// Frag announcement
/// </summary>
class FragAnnouncement final : public Announcement
{
public:
	FragAnnouncement(
		const FName& instigatorPlayerNickname,
		const FName& fraggedPlayerNickname,
		const bool isLocalPlayerInstigator,
		const bool isLocalPlayerFragged
	);
	
	FName GetInstigatorPlayerNickname() const;

	FName GetFraggedPlayerNickname() const;

	bool GetIsLocalPlayerInstigator() const;

	bool GetIsLocalPlayerFragged() const;

private:
	FName _instigatorPlayerNickname;
	
	FName _fraggedPlayerNickname;
	
	bool _isLocalPlayerInstigator;
	
	bool _isLocalPlayerFragged;
};
