#pragma once

#include "GameFramework/GameModeBase.h"
#include "LetsGo/Characters/BotFactory.h"
#include "LetsGo/Characters/PlayerFactory.h"
#include "Misc/TypeContainer.h"
#include "MatchGameMode.generated.h"

///<summary>
///Base game mode for all matches. Has Dependency Injection container.
///</summary>
UCLASS()
class LETSGO_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMatchGameMode();
	
	virtual ~AMatchGameMode();

	TTypeContainer<ESPMode::Fast>* GetDiContainer() const;

	void RegisterSpawnPoint(FTransform spawnPoint);

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;
	
private:
	const int UNDEFINED_INDEX = -1;
	
	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	TArray<FTransform> _spawnPoints;

	int _spawnPointIndex = UNDEFINED_INDEX;

	FTransform GetNextSpawnPoint();
	
	void SpawnPlayers();

	void SpawnLocalPlayerPawn();

	void SpawnBot();

	BotFactory* _botFactory;

	PlayerFactory* _playerFactory;
};
