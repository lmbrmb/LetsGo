#include "LeadAnnouncement.h"

LeadAnnouncement::LeadAnnouncement(const FLeadState leadState) :
	_leadState(leadState)
{
}

FLeadState LeadAnnouncement::GetLeadState() const
{
	return _leadState;
}
