#pragma once

#include "GameFramework/GameModeBase.h"
#include "LetsGo/Avatars/AvatarData.h"
#include "LetsGo/Avatars/AvatarFactory.h"
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
	AMatchGameMode() = default;
	
	virtual ~AMatchGameMode();

	TTypeContainer<ESPMode::Fast>* GetDiContainer() const;

	void RegisterSpawnPoint(FTransform spawnPoint);

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	virtual void OnAvatarDied(AActor* actor);
	
private:
	const int UNDEFINED_INDEX = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _playerRespawnTime = 3.0f;
	
	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	TArray<FTransform> _spawnPoints;

	int _spawnPointIndex = UNDEFINED_INDEX;

	FTransform GetNextSpawnPoint();

	AvatarFactory* _avatarFactory;

	void SpawnAvatar(AvatarData* avatarData);

	UBlueprint* GetAvatarBlueprint(const AvatarType avatarType) const;
	
	TArray<AvatarData*> _avatarsData;

	void RespawnAvatarOnTimer();

	FDelegateHandle	_delegateHandleOnAvatarDied;

	TQueue<AvatarData*> _respawnQueue;
};
