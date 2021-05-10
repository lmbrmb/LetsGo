#pragma once

#include "IDiContainerFactory.h"
#include "Misc/TypeContainer.h"
#include "LetsGo/Data/IUObjectRegistry.h"
#include "LetsGo/AssetFactories/MaterialFactory.h"
#include "LetsGo/AssetFactories/SkeletalMeshFactory.h"
#include "LetsGo/AssetFactories/SkinFactory.h"
#include "LetsGo/Settings/SettingsManagerFactory.h"

/// <summary>
/// Match dependency injection container factory
/// Composes DI container for MatchGameMode
/// </summary>
template <ESPMode Mode>
class MainMenuDiContainerFactory final : IDiContainerFactory<Mode>
{
public:
	MainMenuDiContainerFactory() = default;

	~MainMenuDiContainerFactory() = default;

	virtual TTypeContainer<Mode>* CreateContainer(IUObjectRegistry* uObjectRegistry) override;

private:
	/// <summary>
	/// Disable lazy initialization
	/// </summary>
	const bool LAZY_INITIALIZATION = false;
};

template <ESPMode Mode>
TTypeContainer<Mode>* MainMenuDiContainerFactory<Mode>::CreateContainer(IUObjectRegistry* uObjectRegistry)
{
	auto const container = new TTypeContainer<ESPMode::Fast>();

	auto const materialFactoryInstance = new MaterialFactory(uObjectRegistry, LAZY_INITIALIZATION);
	const TSharedRef<MaterialFactory> materialFactory = MakeShareable(materialFactoryInstance);

	auto const skeletalMeshFactoryInstance = new SkeletalMeshFactory(uObjectRegistry, LAZY_INITIALIZATION);
	const TSharedRef<SkeletalMeshFactory> skeletalMeshFactory = MakeShareable(skeletalMeshFactoryInstance);

	auto const skinFactoryInstance = new SkinFactory(materialFactoryInstance, skeletalMeshFactoryInstance);
	const TSharedRef<SkinFactory> skinFactory = MakeShareable(skinFactoryInstance);

	const TSharedRef<SettingsManagerFactory> settingsManagerFactory = MakeShareable(new SettingsManagerFactory(uObjectRegistry));

	container->RegisterInstance<MaterialFactory>(materialFactory);
	container->RegisterInstance<SkeletalMeshFactory>(skeletalMeshFactory);
	container->RegisterInstance<SkinFactory>(skinFactory);
	container->RegisterInstance<SettingsManagerFactory>(settingsManagerFactory);

	return container;
}
