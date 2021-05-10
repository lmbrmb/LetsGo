#pragma once

#include "MaterialFactory.h"
#include "SkeletalMeshFactory.h"

///<summary>
/// Skin factory
///</summary>
class SkinFactory final
{
	class SkinData final
	{
	public:
		SkinData(const FName skeletalMeshId, const FName materialId):
			SkeletalMeshId(skeletalMeshId),
			MaterialId(materialId)
		{
		}

		FName SkeletalMeshId;
		FName MaterialId;
	};
	
public:
	SkinFactory(MaterialFactory* materialFactory, SkeletalMeshFactory* skeletalMeshFactory);

	~SkinFactory();

	void SetSkin(AActor* actor, const FName& skinId);

	void SetSkin(USkeletalMeshComponent* skeletalMeshComponent, const FName& skinId);

private:
	MaterialFactory* _materialFactory;
	
	SkeletalMeshFactory* _skeletalMeshFactory;
	
	TMap<FName, SkinData*> _skins;

	TMap<FName, USkeletalMesh*> _skeletalMeshes;

	TMap<FName, UMaterialInterface*> _materials;
	
	static const FString ASSET_PATH_PREFIX;
};

Expose_TNameOf(SkinFactory)