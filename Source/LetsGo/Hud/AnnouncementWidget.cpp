#include "AnnouncementWidget.h"

#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementWidget::OnMatchWarmUpAnnouncementRequest()
{
	BpAnnounceMatchWarmUp();
}

void UAnnouncementWidget::OnMatchStartAnnouncementRequest()
{
	BpAnnounceMatchStart();
}

void UAnnouncementWidget::OnMatchEndAnnouncementRequest(const int localPlayerPlace)
{
	BpAnnounceMatchEnd(localPlayerPlace);
}

void UAnnouncementWidget::OnFragAnnouncementRequest(const FragAnnouncement* fragAnnouncement)
{
	AssertIsNotNull(fragAnnouncement);
	
	BpAnnounceFrag(
		fragAnnouncement->GetInstigatorPlayerNickname(),
		fragAnnouncement->GetFraggedPlayerNickname(),
		fragAnnouncement->GetIsLocalPlayerInstigator(),
		fragAnnouncement->GetIsLocalPlayerFragged(),
		fragAnnouncement->GetInstigatorPlayerPlace(),
		fragAnnouncement->GetFraggedPlayerPlace()
	);
}

void UAnnouncementWidget::OnMedalAnnouncementRequest(const MedalAnnouncement* medalAnnouncement)
{
	AssertIsNotNull(medalAnnouncement);
	
	BpAnnounceMedal(medalAnnouncement->GetMedalType());
}

void UAnnouncementWidget::OnAllPlayerAnnouncementsDone()
{
	BpAllPlayerAnnouncementsDone();
}

void UAnnouncementWidget::OnAllMatchAnnouncementsDone()
{
	BpAllMatchAnnouncementsDone();
}
