#include "SkeletalMeshFactory.h"

const FString SkeletalMeshFactory::MIXAMO_PATH_PREFIX = "/Game/Assets/Mixamo/";

SkeletalMeshFactory::SkeletalMeshFactory(const bool lazyInitialization)
{
	Paths.Add("Kachujin", MIXAMO_PATH_PREFIX + "Kachujin/Kachujin_SkeletalMesh");
	Paths.Add("Maria", MIXAMO_PATH_PREFIX + "Maria/Maria_SkeletalMesh");
	Paths.Add("Vampire", MIXAMO_PATH_PREFIX + "Vampire/Vampire_SkeletalMesh");

	if (!lazyInitialization)
	{
		LoadAllAssets();
	}
}
