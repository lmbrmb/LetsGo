#pragma once

#include "Misc/TypeContainer.h"
#include "LetsGo/AssetFactories/MaterialFactory.h"
#include "LetsGo/AssetFactories/SkeletalMeshFactory.h"
#include "LetsGo/AssetFactories/SkinFactory.h"

/// <summary>
/// Match dependency injection container factory
/// Composes DI container for MatchGameMode
/// </summary>
class MainMenuDiContainerFactory final
{
public:
	MainMenuDiContainerFactory() = default;

	~MainMenuDiContainerFactory() = default;

	template<ESPMode Mode = ESPMode::Fast>
	TTypeContainer<Mode>* CreateContainer();

private:
	/// <summary>
	/// Disable lazy initialization. No lag on weapon / health pickup.
	/// TODO: pickups / guns pool
	/// </summary>
	const bool LAZY_INITIALIZATION = false;
};

template <ESPMode Mode>
TTypeContainer<Mode>* MainMenuDiContainerFactory::CreateContainer()
{
	auto const container = new TTypeContainer<Mode>();

	auto const materialFactoryInstance = new MaterialFactory(LAZY_INITIALIZATION);
	auto const skeletalMeshFactoryInstance = new SkeletalMeshFactory(LAZY_INITIALIZATION);
	auto const skinFactoryInstance = new SkinFactory(materialFactoryInstance, skeletalMeshFactoryInstance);
	const TSharedRef<SkinFactory> skinFactory = MakeShareable(skinFactoryInstance);

	container->template RegisterInstance<SkinFactory>(skinFactory);

	return container;
}
