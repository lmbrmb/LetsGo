#include "AnnouncementWidget.h"

void UAnnouncementWidget::OnMessageAnnouncementRequest(const FString& message)
{
	BpAnnounceMessage(message);
}

void UAnnouncementWidget::OnMedalAnnouncementRequest(const FMedalType medalType)
{
	BpAnnounceMedal(medalType);
}
