#include "PlayerHud.h"

void APlayerHud::BeginPlay()
{
	Super::BeginPlay();

	if(_hudBlueprint)
	{
		auto const world = GetWorld();
		_hud = CreateWidget(world, _hudBlueprint);
		_hud->AddToViewport(INT_MIN);
	}
}
