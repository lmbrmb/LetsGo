#pragma once

#include "GameFramework/GameModeBase.h"

#include "LetsGo/Avatars/AvatarData.h"
#include "Misc/TypeContainer.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Avatars/AvatarDataFactory.h"
#include "LetsGo/Avatars/AvatarSpawnFactory.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

#include "MatchGameMode.generated.h"

DECLARE_EVENT_OneParam(AMatchGameMode, EAvatarSpawned, const AAvatar* avatar);

DECLARE_EVENT(AMatchGameMode, EMatchStarted);

///<summary>
///Base game mode for all matches. Provides Dependency Injection container.
///</summary>
UCLASS()
class LETSGO_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	EAvatarSpawned AvatarSpawned;

	EMatchStarted MatchStarted;

	bool GetIsMatchStarted() const;
	
	AMatchGameMode() = default;
	
	virtual ~AMatchGameMode();

	TTypeContainer<ESPMode::Fast>* GetDiContainer() const;

	MatchAnalytics* GetMatchAnalytics() const;
	
	void RegisterSpawnPoint(FTransform spawnPoint);

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	virtual void OnAvatarDied(const UHealthComponent* healthComponent, const float delta);
	
private:
	bool _IsMatchStarted = false;
	
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarRespawnTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarDestroyTime = 2.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchStartDelay = 0.5f;

	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	TArray<FTransform> _spawnPoints;

	int _spawnPointIndex = UNDEFINED_INDEX;
	
	TArray<AvatarData*> _avatarsData;

	AvatarSpawnFactory* _avatarSpawnFactory;
	
	AvatarDataFactory* _avatarDataFactory;

	// PlayerId value
	TQueue<int> _respawnQueue;

	TQueue<AActor*> _destroyQueue;

	MatchAnalytics* _matchAnalytics;

	FTransform GetNextSpawnPoint();

	void SpawnAvatar(AvatarData* avatarData);
	
	void RespawnAvatarOnTimer();

	void DestroyAvatarOnTimer();

	void PopulateAvatarsData();

	void TriggerMatchStart();
};
