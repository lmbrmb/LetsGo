#include "AnnouncementWidget.h"

#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementWidget::OnFragAnnouncementRequest(const FragAnnouncement* fragAnnouncement)
{
	AssertIsNotNull(fragAnnouncement);
	
	BpAnnounceFrag(
		fragAnnouncement->GetInstigatorPlayerNickname(),
		fragAnnouncement->GetFraggedPlayerNickname(),
		fragAnnouncement->GetIsLocalPlayerInstigator(),
		fragAnnouncement->GetIsLocalPlayerFragged()
	);
}

void UAnnouncementWidget::OnMedalAnnouncementRequest(const MedalAnnouncement* medalAnnouncement)
{
	AssertIsNotNull(medalAnnouncement);
	
	BpAnnounceMedal(medalAnnouncement->GetMedalType());
}

void UAnnouncementWidget::OnAllAnnouncementsDone()
{
	BpAllAnnouncementsDone();
}
