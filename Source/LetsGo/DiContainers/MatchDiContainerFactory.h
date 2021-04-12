#pragma once

#include "IDiContainerFactory.h"
#include "LetsGo/Items/HealthItemFactory.h"
#include "LetsGo/Items/GunItemFactory.h"
#include "LetsGo/Items/AmmoItemFactory.h"
#include "LetsGo/AssetFactories/PickupItemFactory.h"
#include "LetsGo/AssetFactories/GunFactory.h"
#include "LetsGo/AssetFactories/AvatarFactory.h"
#include "LetsGo/AssetFactories/MaterialFactory.h"
#include "LetsGo/AssetFactories/SkeletalMeshFactory.h"
#include "LetsGo/AssetFactories/SkinFactory.h"
#include "LetsGo/Avatars/AvatarDataFactory.h"
#include "LetsGo/Avatars/AvatarSpawnFactory.h"
#include "LetsGo/NicknameGenerators/NicknameGeneratorFactory.h"
#include "LetsGo/Forces/ForceFactory.h"
#include "LetsGo/GameModes/GameModeOptionParsers/GameModeOptionParserFactory.h"
#include "LetsGo/Sfx/SfxFactory.h"
#include "LetsGo/Sfx/AvatarSfxFactory.h"
#include "LetsGo/AmmoProviders/AmmoProviderFactory.h"

/// <summary>
/// Match dependency injection container factory
/// Composes DI container for MatchGameMode
/// </summary>
template <ESPMode Mode>
class MatchDiContainerFactory final : IDiContainerFactory<Mode>
{
public:
	MatchDiContainerFactory() = default;

	~MatchDiContainerFactory() = default;

	virtual TTypeContainer<Mode>* CreateContainer(IUObjectRegistry* uObjectRegistry) override;

private:
	/// <summary>
	/// Disable lazy initialization. No lag on weapon / health pickup.
	/// TODO: pickups / guns pool
	/// </summary>
	const bool LAZY_INITIALIZATION = false;
};

template <ESPMode Mode>
TTypeContainer<Mode>* MatchDiContainerFactory<Mode>::CreateContainer(IUObjectRegistry* uObjectRegistry)
{
	auto const container = new TTypeContainer<Mode>();

	const TSharedRef<PickupItemFactory> pickupItemFactory = MakeShareable(new PickupItemFactory(uObjectRegistry, LAZY_INITIALIZATION));
	const TSharedRef<GunItemFactory> gunItemFactory = MakeShareable(new GunItemFactory());
	const TSharedRef<HealthItemFactory> healthItemFactory = MakeShareable(new HealthItemFactory());
	const TSharedRef<AmmoItemFactory> ammoItemFactory = MakeShareable(new AmmoItemFactory());
	const TSharedRef<GunFactory> gunFactory = MakeShareable(new GunFactory(uObjectRegistry, LAZY_INITIALIZATION));
	const TSharedRef<ForceFactory> forceFactory = MakeShareable(new ForceFactory());
	const TSharedRef<AmmoProviderFactory> ammoProviderFactory = MakeShareable(new AmmoProviderFactory());

	auto const nicknameGeneratorFactoryInstance = new NicknameGeneratorFactory();
	const TSharedRef<NicknameGeneratorFactory> nicknameGeneratorFactory = MakeShareable(nicknameGeneratorFactoryInstance);;

	const auto nicknameGenerator = nicknameGeneratorFactoryInstance->Create();
	const TSharedRef<AvatarDataFactory> avatarDataFactory = MakeShareable(new AvatarDataFactory(nicknameGenerator));

	auto const avatarFactoryInstance = new AvatarFactory(uObjectRegistry, LAZY_INITIALIZATION);
	const TSharedRef<AvatarFactory> avatarFactory = MakeShareable(avatarFactoryInstance);

	auto const materialFactoryInstance = new MaterialFactory(uObjectRegistry, LAZY_INITIALIZATION);
	const TSharedRef<MaterialFactory> materialFactory = MakeShareable(materialFactoryInstance);

	auto const skeletalMeshFactoryInstance = new SkeletalMeshFactory(uObjectRegistry, LAZY_INITIALIZATION);
	const TSharedRef<SkeletalMeshFactory> skeletalMeshFactory = MakeShareable(skeletalMeshFactoryInstance);

	auto const skinFactoryInstance = new SkinFactory(materialFactoryInstance, skeletalMeshFactoryInstance);
	const TSharedRef<SkinFactory> skinFactory = MakeShareable(skinFactoryInstance);

	const TSharedRef<AvatarSpawnFactory> avatarSpawnFactory = MakeShareable(new AvatarSpawnFactory(avatarFactoryInstance, skinFactoryInstance));
	const TSharedRef<GameModeOptionParserFactory> gameModeOptionParserFactory = MakeShareable(new GameModeOptionParserFactory());

	auto const sfxFactoryInstance = new SfxFactory(uObjectRegistry, LAZY_INITIALIZATION);
	const TSharedRef<SfxFactory> sfxFactory = MakeShareable(sfxFactoryInstance);
	const TSharedRef<AvatarSfxFactory> avatarSfxFactory = MakeShareable(new AvatarSfxFactory(sfxFactoryInstance));

	container->template RegisterInstance<PickupItemFactory>(pickupItemFactory);
	container->template RegisterInstance<GunItemFactory>(gunItemFactory);
	container->template RegisterInstance<AmmoItemFactory>(ammoItemFactory);
	container->template RegisterInstance<HealthItemFactory>(healthItemFactory);
	container->template RegisterInstance<GunFactory>(gunFactory);
	container->template RegisterInstance<ForceFactory>(forceFactory);
	container->template RegisterInstance<AmmoProviderFactory>(ammoProviderFactory);
	container->template RegisterInstance<AvatarDataFactory>(avatarDataFactory);
	container->template RegisterInstance<NicknameGeneratorFactory>(nicknameGeneratorFactory);
	container->template RegisterInstance<MaterialFactory>(materialFactory);
	container->template RegisterInstance<SkeletalMeshFactory>(skeletalMeshFactory);
	container->template RegisterInstance<SkinFactory>(skinFactory);
	container->template RegisterInstance<AvatarFactory>(avatarFactory);
	container->template RegisterInstance<AvatarSpawnFactory>(avatarSpawnFactory);
	container->template RegisterInstance<GameModeOptionParserFactory>(gameModeOptionParserFactory);
	container->template RegisterInstance<SfxFactory>(sfxFactory);
	container->template RegisterInstance<AvatarSfxFactory>(avatarSfxFactory);

	return container;
}
