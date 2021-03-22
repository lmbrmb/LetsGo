#include "AnnouncementToHudMapping.h"

#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Hud/AnnouncementWidget.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementToHudMapping::Map()
{
	auto const owner = GetOwner();
	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	auto const playerController = Cast<APlayerController>(owner);
	AssertIsNotNull(playerController);

	auto const hud = playerController->GetHUD();
	AssertIsNotNull(hud);

	_playerHud = Cast<APlayerHud>(hud);
	AssertIsNotNull(_playerHud);

	if (_playerHud->IsInitialized())
	{
		OnPlayerHudInitialized();
		return;
	}

	_playerHud->Initialized.AddUObject(this, &UAnnouncementToHudMapping::OnPlayerHudInitialized);
}

void UAnnouncementToHudMapping::OnPlayerHudInitialized()
{
	_playerHud->Initialized.RemoveAll(this);
	
	auto const hudWidget = _playerHud->GetHudWidget();
	AssertIsNotNull(hudWidget);

	auto const hudRootWidget = hudWidget->GetRootWidget();
	AssertIsNotNull(hudRootWidget);

	auto const panelWidget = Cast<UPanelWidget>(hudRootWidget);
	AssertIsNotNull(panelWidget);

	auto announcementWidgetCount = 0;
	auto const childWidgets = panelWidget->GetAllChildren();
	UAnnouncementWidget* theAnnouncementWidget = nullptr;
	
	for (auto widget : childWidgets)
	{
		auto const announcementWidget = Cast<UAnnouncementWidget>(widget);
		if(announcementWidget)
		{
			announcementWidgetCount++;
			theAnnouncementWidget = announcementWidget;
		}
	}

	AssertIsNotNull(theAnnouncementWidget);
	AssertIsLessOrEqual(announcementWidgetCount, 1);

	_announcementManagerComponent->MatchWarmUpAnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnMatchWarmUpAnnouncementRequest);
	_announcementManagerComponent->MatchStartAnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnMatchStartAnnouncementRequest);
	_announcementManagerComponent->MatchEndAnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnMatchEndAnnouncementRequest);
	_announcementManagerComponent->FragAnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnFragAnnouncementRequest);
	_announcementManagerComponent->MedalAnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnMedalAnnouncementRequest);
	_announcementManagerComponent->AllAnnouncementsDone.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnAllAnnouncementsDone);
	StartDestroyTask();
}
