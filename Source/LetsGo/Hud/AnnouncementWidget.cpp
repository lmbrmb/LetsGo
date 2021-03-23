#include "AnnouncementWidget.h"

#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementWidget::OnMatchWarmUpAnnouncementRequest()
{
	BpAnnounceMatchWarmUp();
}

void UAnnouncementWidget::OnMatchStartAnnouncementRequest()
{
	BpAnnounceMatchStart();
}

void UAnnouncementWidget::OnMatchEndAnnouncementRequest()
{
	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	auto const isLocalPlayerWonMatch = matchGameMode->IsLocalPlayerWonMatch();
	BpAnnounceMatchEnd(isLocalPlayerWonMatch);
}

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

void UAnnouncementWidget::OnAllPlayerAnnouncementsDone()
{
	BpAllPlayerAnnouncementsDone();
}

void UAnnouncementWidget::OnAllMatchAnnouncementsDone()
{
	BpAllMatchAnnouncementsDone();
}
