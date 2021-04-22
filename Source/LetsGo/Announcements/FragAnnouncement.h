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
		const bool isLocalPlayerFragged,
		const int instigatorPlayerPlace,
		const int fraggedPlayerPlace
	);

	const FName& GetInstigatorPlayerNickname() const;

	const FName& GetFraggedPlayerNickname() const;

	bool GetIsLocalPlayerInstigator() const;

	bool GetIsLocalPlayerFragged() const;

	int GetInstigatorPlayerPlace() const;

	int GetFraggedPlayerPlace() const;

private:
	FName _instigatorPlayerNickname;

	FName _fraggedPlayerNickname;

	bool _isLocalPlayerInstigator;

	bool _isLocalPlayerFragged;

	int _instigatorPlayerPlace;

	int _fraggedPlayerPlace;
};
