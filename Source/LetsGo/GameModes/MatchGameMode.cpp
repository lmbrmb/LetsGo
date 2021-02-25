#include "MatchGameMode.h"
#include "LetsGo/MatchDependencyInjectionContainerFactory.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssetUtils.h"

const int BOT_COUNT = 3;

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

	auto const playerFactory = GetDiContainer()->GetInstance<AvatarFactory>();
	_avatarFactory = &playerFactory.Get();

	_avatarsData.Add(new AvatarData(FGuid::NewGuid(), AvatarType::LocalPlayer));
	for(auto i = 0; i<BOT_COUNT; i++)
	{
		_avatarsData.Add(new AvatarData(FGuid::NewGuid(), AvatarType::Bot));
	}
}

TTypeContainer<ESPMode::Fast>* AMatchGameMode::GetDiContainer() const
{
	return _diContainer;
}

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (auto avatarData : _avatarsData)
	{
		SpawnAvatar(avatarData);
	}
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

void AMatchGameMode::SpawnAvatar(AvatarData* avatarData)
{
	auto const avatarType = avatarData->GetAvatarType();
	auto const avatarBlueprint = GetAvatarBlueprint(avatarType);

	if (!avatarBlueprint)
	{
		DevLogger::GetLoggingChannel()->Log("Avatar blueprint is null", LogSeverity::Error);
		return;
	}

	auto const avatar = AssetUtils::SpawnBlueprint<AAvatar>(GetWorld(), nullptr, avatarBlueprint, GetNextSpawnPoint());
	avatar->SetAvatarData(avatarData);
	auto const healthComponent = avatar->FindComponentByClass<UHealthComponent>();
	healthComponent->Died.AddUObject(this, &AMatchGameMode::OnAvatarDied);
}

UBlueprint* AMatchGameMode::GetAvatarBlueprint(const AvatarType avatarType) const
{
	switch (avatarType)
	{
		case AvatarType::LocalPlayer:
			return  _avatarFactory->GetLocalPlayerBlueprint();

		case AvatarType::RemotePlayer:
			return  _avatarFactory->GetRemotePlayerBlueprint();

		case AvatarType::Bot:
			return _avatarFactory->GetBotBlueprint();
	}
	return nullptr;
}

void AMatchGameMode::OnAvatarDied(AActor* actor)
{
	actor->Destroy();
}
