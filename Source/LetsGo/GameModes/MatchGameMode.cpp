#include "MatchGameMode.h"
#include "LetsGo/MatchDependencyInjectionContainerFactory.h"
#include "LetsGo/Characters/ProtagonistPawn.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssetUtils.h"

AMatchGameMode::AMatchGameMode()
{
	//Do nothing
}

AMatchGameMode::~AMatchGameMode()
{
	delete _diContainer;
}

void AMatchGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	MatchDependencyInjectionContainerFactory containerFactory;
	_diContainer = containerFactory.CreateContainer<ESPMode::Fast>();
}

TTypeContainer<ESPMode::Fast>* AMatchGameMode::GetDiContainer() const
{
	return _diContainer;
}

void AMatchGameMode::BeginPlay()
{
	auto const botFactory = GetDiContainer()->GetInstance<BotFactory>();
	_botFactory = &botFactory.Get();

	auto const playerFactory = GetDiContainer()->GetInstance<PlayerFactory>();
	_playerFactory = &playerFactory.Get();

	SpawnPlayers();
}

void AMatchGameMode::RegisterSpawnPoint(FTransform spawnPoint)
{
	_spawnPoints.Add(spawnPoint);
}

FTransform AMatchGameMode::GetNextSpawnPoint()
{
	auto const spawnPointCount = _spawnPoints.Num();
	
	if(spawnPointCount <= 0)
	{
		DevLogger::GetLoggingChannel()->Log("No spawn points registered", LogSeverity::Error);
		return FTransform();
	}
	
	auto nextIndex = _spawnPointIndex + 1;

	if (nextIndex >= spawnPointCount)
	{
		nextIndex = 0;
	}

	_spawnPointIndex = nextIndex;
	return _spawnPoints[_spawnPointIndex];
}

void AMatchGameMode::SpawnPlayers()
{
	//TODO: get actual info
	auto const botsCount = 3;

	SpawnLocalPlayerPawn();

	for (auto i = 0; i < botsCount; i++)
	{
		SpawnBot();
	}
}

void AMatchGameMode::SpawnLocalPlayerPawn()
{
	auto const playerBlueprint = _playerFactory->GetBlueprint("Player1");
	AssetUtils::SpawnBlueprint<AProtagonistPawn>(GetWorld(), nullptr, playerBlueprint, GetNextSpawnPoint());
}

void AMatchGameMode::SpawnBot()
{
	//TODO: bot skill
	auto const botBlueprint = _botFactory->GetBlueprint("Bot1");
	AssetUtils::SpawnBlueprint<AActor>(GetWorld(), nullptr, botBlueprint, GetNextSpawnPoint());
}
