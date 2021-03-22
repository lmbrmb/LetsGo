#include "MatchStateWidget.h"
#include "LetsGo/Utils/AssertUtils.h"

void UMatchStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(_matchGameMode);

	if (_matchGameMode->IsMatchEnded())
	{
		OnMatchEnd();
		return;
	}

	_matchGameMode->MatchEnd.AddUObject(this, &UMatchStateWidget::OnMatchEnd);

	if (_matchGameMode->IsMatchStarted())
	{
		OnMatchStart();
		return;
	}

	_matchGameMode->MatchStart.AddUObject(this, &UMatchStateWidget::OnMatchStart);

	if (_matchGameMode->IsMatchWarmUp())
	{
		OnMatchWarmUp();
	}
	else
	{
		_matchGameMode->MatchWarmUp.AddUObject(this, &UMatchStateWidget::OnMatchWarmUp);
	}
}

void UMatchStateWidget::OnMatchWarmUp()
{
	BpOnMatchWarmUp();
}

void UMatchStateWidget::OnMatchStart()
{
	BpOnMatchStart();
}

void UMatchStateWidget::OnMatchEnd()
{
	BpOnMatchEnd();
}

float UMatchStateWidget::GetMatchTime() const
{
	if (!_matchGameMode)
	{
		return 0;
	}

	return _matchGameMode->GetMatchTime();
}
