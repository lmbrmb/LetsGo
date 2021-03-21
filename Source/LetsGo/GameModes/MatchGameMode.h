#pragma once

#include "GameFramework/GameModeBase.h"

#include "LetsGo/Avatars/AvatarData.h"
#include "Misc/TypeContainer.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Avatars/AvatarDataFactory.h"
#include "LetsGo/Avatars/AvatarSpawnFactory.h"
#include "LetsGo/GameStates/MatchState.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

#include "MatchGameMode.generated.h"

DECLARE_EVENT_OneParam(AMatchGameMode, EAvatarSpawned, const AAvatar* avatar);

DECLARE_EVENT_FourParams(
AMatchGameMode,
EPlayerFragged,
const PlayerId& instigatorPlayerId,
const PlayerId& fraggedPlayerId,
const FName& instigatorPlayerNickname,
const FName& fraggedPlayerNickname
);

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

	EPlayerFragged PlayerFragged;

	EMatchStarted MatchStarted;

	AMatchGameMode() = default;
	
	virtual ~AMatchGameMode();

	TTypeContainer<ESPMode::Fast>* GetDiContainer() const;

	MatchAnalytics* GetMatchAnalytics() const;
	
	void RegisterSpawnPoint(FTransform spawnPoint);

	float GetMatchTime() const;

	float GetCurrentStateTime() const;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	virtual void OnAvatarDied(const UHealthComponent* healthComponent, const float delta);
	
private:
	float _stateStartTime;

	float _matchEndTime;
	
	void SetMatchState(MatchState matchState);

	bool CanEnterState(MatchState matchState) const;
	
	MatchState _matchState = MatchState::None;

	FTimerHandle _matchTimerHandle;
	
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarRespawnTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarDestroyTime = 2.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _warmupDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchDuration = 5.0f;

	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	TArray<FTransform> _spawnPoints;

	int _spawnPointIndex = UNDEFINED_INDEX;
	
	TMap<int, AvatarData*> _avatarsData;

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

	void TriggerMatchEnd();

	AvatarData* GetAvatarData(const int playerIdValue) const;

	AvatarData* GetAvatarData(const PlayerId& playerIdValue) const;
};
