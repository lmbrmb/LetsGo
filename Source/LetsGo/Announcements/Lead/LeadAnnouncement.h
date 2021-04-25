#pragma once

#include "LeadState.h"
#include "LetsGo/Announcements/IAnnouncement.h"

/// <summary>
/// Lead announcement
/// </summary>
class LeadAnnouncement final : public IAnnouncement
{
public:
	explicit LeadAnnouncement(const FLeadState leadState);

	FLeadState GetLeadState() const;

private:
	FLeadState _leadState;
};
