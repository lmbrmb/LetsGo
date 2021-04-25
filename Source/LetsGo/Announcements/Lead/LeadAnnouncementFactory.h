#pragma once

#include "LeadState.h"
#include "LetsGo/Announcements/IAnnouncement.h"
#include "LetsGo/Announcements/IAnnouncementFactory.h"

///<summary>
/// Lead announcement factory
///</summary>
class LeadAnnouncementFactory final : public IAnnouncementFactory
{
public:
	IAnnouncement* Create(
		const FLeadState& leadState
	);
};
