#pragma once

#include "GameFramework/GameModeBase.h"

#include "LetsGo/Avatars/AvatarData.h"
#include "Misc/TypeContainer.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Avatars/AvatarSpawnFactory.h"
#include "LetsGo/GameStates/MatchState.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/SpawnPoints/SpawnPointType.h"

#include "MatchGameMode.generated.h"

DECLARE_EVENT(AMatchGameMode, EMatchWarmUp);

DECLARE_EVENT(AMatchGameMode, EMatchStart);

DECLARE_EVENT(AMatchGameMode, EMatchEnd);

DECLARE_EVENT_OneParam(AMatchGameMode, EAvatarSpawned, const AAvatar* avatar);

DECLARE_EVENT_FourParams(
AMatchGameMode,
EPlayerFragged,
const PlayerId& instigatorPlayerId,
const PlayerId& fraggedPlayerId,
const FName& instigatorPlayerNickname,
const FName& fraggedPlayerNickname
);

///<summary>
///Base game mode for all matches. Provides Dependency Injection container.
///</summary>
UCLASS()
class LETSGO_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	EMatchWarmUp MatchWarmUp;

	EMatchStart MatchStart;

	EMatchEnd MatchEnd;
	
	EAvatarSpawned AvatarSpawned;

	EPlayerFragged PlayerFragged;
	
	AMatchGameMode() = default;
	
	virtual ~AMatchGameMode();

	TTypeContainer<ESPMode::Fast>* GetDiContainer() const;

	MatchAnalytics* GetMatchAnalytics() const;
	
	void RegisterSpawnPoint(const FSpawnPointType type, const FTransform& transform);

	float GetMatchTime() const;

	bool IsMatchWarmUp() const;
	
	bool IsMatchStarted() const;
	
	bool IsMatchEnded() const;

	bool IsMatchInProgress() const;

	virtual bool IsLocalPlayerWonMatch();
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override final;

	virtual void PopulateAvatarsData();

	virtual void OnFragsCountChanged();

	void SetMatchState(MatchState matchState);
	
	/// <summary>
	/// PlayerId value, Avatar data
	/// </summary>
	TMap<int, AvatarData*> AvatarsData;

	/// <summary>
	/// PlayerId value, frag count
	/// </summary>
	TMap<int, int> Frags;
	
private:
	float _stateStartTime = 0;

	float _matchEndTime = 0;

	bool CanEnterState(MatchState matchState) const;

	float GetCurrentStateTime() const;
	
	MatchState _matchState = MatchState::None;

	FTimerHandle _matchStateTimerHandle;
	
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarRespawnTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarDestroyTime = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _warmUpDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _warmUpDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchDuration = 5.0f;

	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	int _playerSpawnPointIndex = UNDEFINED_INDEX;

	TArray<FTransform> _playerSpawnPoints;

	TArray<FTransform> _weaponSpawnPoints;

	TArray<FTransform> _healthSpawnPoints;

	AvatarSpawnFactory* _avatarSpawnFactory;

	// PlayerId value
	TQueue<int> _respawnQueue;

	TQueue<AActor*> _destroyQueue;

	MatchAnalytics* _matchAnalytics;

	void OnAvatarDied(const UHealthComponent* healthComponent, const float delta);
	
	FTransform GetNextSpawnPoint();

	void SpawnAvatar(AvatarData* avatarData);
	
	void RespawnAvatarOnTimer();

	void DestroyAvatarOnTimer();

	void TriggerMatchWarmUp();
	
	void TriggerMatchStart();

	void TriggerMatchEnd();

	AvatarData* GetAvatarData(const int playerIdValue) const;

	AvatarData* GetAvatarData(const PlayerId& playerIdValue) const;
};
