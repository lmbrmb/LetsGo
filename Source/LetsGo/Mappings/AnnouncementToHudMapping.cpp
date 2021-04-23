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

	if(_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
	}
	else
	{
		_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementToHudMapping::OnAnnouncementManagerComponentInitialized);
	}

	auto const playerController = Cast<APlayerController>(owner);
	AssertIsNotNull(playerController);

	auto const hud = playerController->GetHUD();
	AssertIsNotNull(hud);

	_playerHud = Cast<APlayerHud>(hud);
	AssertIsNotNull(_playerHud);

	if (_playerHud->IsInitialized())
	{
		OnPlayerHudInitialized();
	}
	else
	{
		_playerHud->Initialized.AddUObject(this, &UAnnouncementToHudMapping::OnPlayerHudInitialized);
	}
}

void UAnnouncementToHudMapping::OnPlayerHudInitialized()
{
	_playerHud->Initialized.RemoveAll(this);
	_isPlayerHudInitialized = true;
	OnPartialInitialization();
}

void UAnnouncementToHudMapping::OnAnnouncementManagerComponentInitialized()
{
	_announcementManagerComponent->Initialized.RemoveAll(this);
	_isAnnouncementManagerComponentInitialized = true;
	OnPartialInitialization();
}

void UAnnouncementToHudMapping::OnPartialInitialization()
{
	if(!_isPlayerHudInitialized || !_isAnnouncementManagerComponentInitialized)
	{
		return;
	}
	
	Bind();
}

void UAnnouncementToHudMapping::Bind()
{
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

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	announcementManager->AnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnAnnouncementRequest);
	announcementManager->AllAnnouncementsDone.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnAllAnnouncementsDone);

	StartDestroyTask();
}
