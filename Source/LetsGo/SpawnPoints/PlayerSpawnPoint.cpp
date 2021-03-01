#include "PlayerSpawnPoint.h"
#include "LetsGo/GameModes/MatchGameMode.h"

APlayerSpawnPoint::APlayerSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	matchGameMode->RegisterSpawnPoint(GetTransform());
}
