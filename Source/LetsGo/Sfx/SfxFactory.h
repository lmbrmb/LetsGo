#pragma once

#include "LetsGo/AssetFactories/CachedAssetFactory.h"

class SfxFactory final : public CachedAssetFactory<USoundBase>
{
public:
	explicit SfxFactory(const bool lazyInitialization);

private:
	static int FirstStepIndex;

	static int StepsInGroup;

	static FString Q3AssetPath;

	static FString StepsAssetPath;

	static FString CharactersAssetPath;
};

Expose_TNameOf(SfxFactory)
