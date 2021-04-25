#pragma once

#include "ProjectGameModeBase.h"

#include "LetsGo/Avatars/AvatarData.h"
#include "Misc/TypeContainer.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/Avatars/AvatarSpawnFactory.h"
#include "LetsGo/Data/TeamId.h"
#include "LetsGo/GameStates/MatchState.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Settings/PlayerSettingsManager.h"
#include "LetsGo/SpawnPoints/SpawnPointType.h"

#include "MatchGameMode.generated.h"

DECLARE_EVENT(AMatchGameMode, EMatchWarmUp);

DECLARE_EVENT(AMatchGameMode, EMatchStart);

DECLARE_EVENT(AMatchGameMode, EMatchEnd);

DECLARE_EVENT_OneParam(AMatchGameMode, EAvatarSpawned, const AAvatar* avatar);

DECLARE_EVENT_TwoParams(
AMatchGameMode,
EPlayerFragged,
const PlayerId& instigatorPlayerId,
const PlayerId& fraggedPlayerId
);

///<summary>
///Base game mode for all matches.
///</summary>
UCLASS()
class LETSGO_API AMatchGameMode : public AProjectGameModeBase
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

	// IDiContainerProvider implementation
	virtual TTypeContainer<ESPMode::Fast>* GetDiContainer() const override;

	MatchAnalytics* GetMatchAnalytics() const;
	
	void RegisterSpawnPoint(const FSpawnPointType type, const FTransform& transform);

	float GetMatchTime() const;

	bool IsMatchWarmUp() const;
	
	bool IsMatchStarted() const;
	
	bool IsMatchEnded() const;

	bool IsMatchInProgress() const;

	virtual bool IsLocalPlayerWonMatch();

	int GetFragLimit() const;

	void SetFragLimit(const int fragLimit);

	int GetBotCount() const;

	void SetBotCount(const int botCount);

	const SkinId& GetLocalPlayerSkinId() const;

	void SetLocalPlayerSkinId(const SkinId& localPlayerSkinId);

	float GetMatchDuration() const;

	void SetMatchDuration(const float matchDuration);

	virtual int GetPlayerPlace(const PlayerId& playerId) const;

	int GetPlayerFragCount(const PlayerId& playerId) const;

	int GetTeamFragCount(const TeamId& teamId) const;

	TeamId GetPlayerTeamId(const PlayerId& playerId) const;

	FName GetPlayerNickname(const PlayerId& playerId) const;

	const TArray<AAvatar*>& GetAvatars() const;

	PlayerSettingsManager* GetPlayerSettingsManager() const;

	const TMap<int, int>& GetPlayerFrags() const;

	const TMap<int, int>& GetTeamFrags() const;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override final;

	virtual void PopulateAvatarsData();

	virtual void OnFragsCountChanged();

	void TriggerMatchEnd();
	
	/// <summary>
	/// PlayerId value, Avatar data
	/// </summary>
	TMap<int, AvatarData*> AvatarsData;

	/// <summary>
	/// PlayerId value, frag count
	/// </summary>
	TMap<int, int> PlayerFrags;

	/// <summary>
	/// TeamId value, frag count
	/// </summary>
	TMap<int, int> TeamFrags;

	AvatarData* GetAvatarData(const int playerIdValue) const;

	AvatarData* GetAvatarData(const PlayerId& playerIdValue) const;

private:
	float _stateStartTime = 0;

	float _matchEndTime = 0;

	bool CanEnterState(MatchState matchState) const;

	float GetCurrentStateTime() const;
	
	MatchState _matchState = MatchState::None;

	FTimerHandle _matchStateTimerHandle;
	
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	int _fragLimit = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	int _botCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _localPlayerSkinIdValue = "Kachujin";

	SkinId _localPlayerSkinId = SkinId(_localPlayerSkinIdValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarRespawnTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarDestroyTime = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _warmUpDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchDuration = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _exitToMainMenuDelay = 3.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _mainMenuLevelName = "MainMenu";

	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	int _playerSpawnPointIndex = UNDEFINED_INDEX;

	TArray<FTransform> _playerSpawnPoints;

	TArray<FTransform> _weaponSpawnPoints;

	TArray<FTransform> _healthSpawnPoints;

	AvatarSpawnFactory* _avatarSpawnFactory;

	// PlayerId value
	TQueue<int> _respawnQueue;

	TQueue<AAvatar*> _avatarDestroyQueue;

	TArray<AAvatar*> _avatars;

	MatchAnalytics* _matchAnalytics;

	void OnAvatarDied(UHealthComponent* healthComponent, const float delta);

	FTransform GetNextSpawnPoint();

	void ParseMatchOptions(const FString& options);

	void SpawnAvatar(AvatarData* avatarData);

	void RespawnAvatarOnTimer();

	void DestroyAvatarOnTimer();

	void SetMatchState(MatchState matchState);

	void TriggerMatchWarmUp();

	void TriggerMatchStart();

	void TriggerExitToMainMenu();

	PlayerSettingsManager* _playerSettingsManager;
};
