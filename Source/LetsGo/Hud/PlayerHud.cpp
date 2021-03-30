#include "PlayerHud.h"

#include "LetsGo/Utils/AssertUtils.h"

void APlayerHud::BeginPlay()
{
	Super::BeginPlay();

	AssertIsNotNull(_hudWidgetBlueprint);
	
	auto const world = GetWorld();
	_hudWidget = CreateWidget(world, _hudWidgetBlueprint);
	_hudWidget->AddToViewport(INT_MIN);
	
	Initialized.Broadcast();
}

UUserWidget* APlayerHud::GetHudWidget() const
{
	return _hudWidget;
}

bool APlayerHud::IsInitialized() const
{
	return _hudWidget != nullptr;
}
