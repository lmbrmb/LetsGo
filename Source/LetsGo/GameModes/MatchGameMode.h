#pragma once

#include "GameFramework/GameModeBase.h"

#include "LetsGo/Avatars/AvatarData.h"
#include "LetsGo/Avatars/AvatarFactory.h"
#include "Misc/TypeContainer.h"
#include "LetsGo/Analytics/MatchAnalytics.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

#include "MatchGameMode.generated.h"

///<summary>
///Base game mode for all matches. Has Dependency Injection container.
///</summary>
UCLASS()
class LETSGO_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(
	MatchAnalytics,
		EAvatarSpawned,
		const AAvatar* avatar
		);

	EAvatarSpawned AvatarSpawned;
	
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
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarRespawnTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _avatarDestroyTime = 0.5f;
	
	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	TArray<FTransform> _spawnPoints;

	int _spawnPointIndex = UNDEFINED_INDEX;
	
	TArray<AvatarData*> _avatarsData;

	AvatarFactory* _avatarFactory;

	TQueue<int32> _respawnQueue;

	TQueue<AActor*> _destroyQueue;

	MatchAnalytics* _matchAnalytics;

	FTransform GetNextSpawnPoint();

	void SpawnAvatar(AvatarData* avatarData);

	UBlueprint* GetAvatarBlueprint(const AvatarType avatarType) const;
	
	void RespawnAvatarOnTimer();

	void DestroyAvatarOnTimer();

	void PopulateAvatarsData();
};
