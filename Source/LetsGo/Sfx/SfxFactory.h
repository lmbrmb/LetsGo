#pragma once

#include "LetsGo/AssetFactories/CachedAssetFactory.h"

class SfxFactory final : public CachedAssetFactory<USoundBase>
{
public:
	SfxFactory();

private:
	static int FirstStepIndex;

	static int StepsInGroup;

	static FString StepsAssetPath;

	static FString JumpsAssetPath;
};

Expose_TNameOf(SfxFactory)
