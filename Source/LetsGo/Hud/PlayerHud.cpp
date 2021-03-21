#include "PlayerHud.h"

void APlayerHud::BeginPlay()
{
	Super::BeginPlay();
	
	if(_hudBlueprint)
	{
		auto const world = GetWorld();
		_hudWidget = CreateWidget(world, _hudBlueprint);
		_hudWidget->AddToViewport(INT_MIN);
		
		Initialized.Broadcast();
	}
}

UUserWidget* APlayerHud::GetHudWidget() const
{
	return _hudWidget;
}

bool APlayerHud::IsInitialized() const
{
	return _hudWidget != nullptr;
}
