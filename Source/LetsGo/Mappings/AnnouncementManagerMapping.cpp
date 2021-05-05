#include "AnnouncementManagerMapping.h"

#include "LetsGo/Announcements/AnnouncementManagerComponent.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Hud/AnnouncementWidget.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAnnouncementManagerMapping::Map()
{
	auto const owner = GetOwner();
	_announcementManagerComponent = owner->FindComponentByClass<UAnnouncementManagerComponent>();
	AssertIsNotNull(_announcementManagerComponent);

	if (_announcementManagerComponent->IsInitialized())
	{
		OnAnnouncementManagerComponentInitialized();
	}
	else
	{
		_announcementManagerComponent->Initialized.AddUObject(this, &UAnnouncementManagerMapping::OnAnnouncementManagerComponentInitialized);
	}

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	_matchGameMode->Exit.AddUObject(this, &UAnnouncementManagerMapping::OnExit);

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
		_playerHud->Initialized.AddUObject(this, &UAnnouncementManagerMapping::OnPlayerHudInitialized);
	}
}

void UAnnouncementManagerMapping::OnAnnouncementManagerComponentInitialized()
{
	_announcementManagerComponent->Initialized.RemoveAll(this);
	_isAnnouncementManagerComponentInitialized = true;
	OnPartialInitialization();
}

void UAnnouncementManagerMapping::OnPlayerHudInitialized()
{
	_playerHud->Initialized.RemoveAll(this);
	_isPlayerHudInitialized = true;
	OnPartialInitialization();
}

void UAnnouncementManagerMapping::OnPartialInitialization() const
{
	if(!_isAnnouncementManagerComponentInitialized || !_isPlayerHudInitialized)
	{
		return;
	}

	Bind();
}

void UAnnouncementManagerMapping::Bind() const
{
	AssertIsNotNull(_announcementManagerComponent);
	
	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	AssertIsNotNull(_matchGameMode);

	_matchGameMode->PlayerFragged.AddRaw(announcementManager, &IAnnouncementManager::OnPlayerFragged);

	auto const matchAnalytics = _matchGameMode->GetMatchAnalytics();
	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MedalAchieved.AddRaw(announcementManager, &IAnnouncementManager::OnMedalAchieved);

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
		if (announcementWidget)
		{
			announcementWidgetCount++;
			theAnnouncementWidget = announcementWidget;
		}
	}

	AssertIsNotNull(theAnnouncementWidget);
	AssertIsLessOrEqual(announcementWidgetCount, 1);

	announcementManager->AnnouncementRequest.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnAnnouncementRequest);
	announcementManager->AllAnnouncementsDone.AddUObject(theAnnouncementWidget, &UAnnouncementWidget::OnAllAnnouncementsDone);

	if(_matchGameMode->IsMatchEnded())
	{
		OnMatchEnd();
		return;
	}

	_matchGameMode->MatchEnd.AddUObject(this, &UAnnouncementManagerMapping::OnMatchEnd);

	if (_matchGameMode->IsMatchStarted())
	{
		announcementManager->OnMatchStart();
		return;
	}

	_matchGameMode->MatchStart.AddRaw(announcementManager, &IAnnouncementManager::OnMatchStart);

	if (_matchGameMode->IsMatchWarmUp())
	{
		announcementManager->OnMatchWarmUp();
		return;
	}

	_matchGameMode->MatchWarmUp.AddRaw(announcementManager, &IAnnouncementManager::OnMatchWarmUp);
}

void UAnnouncementManagerMapping::UnBind() const
{
	AssertIsNotNull(_announcementManagerComponent);
	
	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	AssertIsNotNull(announcementManager);

	AssertIsNotNull(_matchGameMode);
	_matchGameMode->PlayerFragged.RemoveAll(announcementManager);
	_matchGameMode->MatchStart.RemoveAll(announcementManager);
	_matchGameMode->MatchWarmUp.RemoveAll(announcementManager);

	auto const matchAnalytics = _matchGameMode->GetMatchAnalytics();
	AssertIsNotNull(matchAnalytics);

	matchAnalytics->MedalAchieved.RemoveAll(announcementManager);
	announcementManager->ClearAllAnnouncements();
}

void UAnnouncementManagerMapping::OnMatchEnd() const
{
	_matchGameMode->MatchEnd.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	if (!announcementManager)
	{
		return;
	}

	UnBind();
	announcementManager->ClearAllAnnouncements();
	announcementManager->OnMatchEnd();
}

void UAnnouncementManagerMapping::OnExit() const
{
	_matchGameMode->Exit.RemoveAll(this);

	auto const announcementManager = _announcementManagerComponent->GetAnnouncementManager();
	if (!announcementManager)
	{
		return;
	}

	UnBind();
	announcementManager->ClearAllAnnouncements();
}
