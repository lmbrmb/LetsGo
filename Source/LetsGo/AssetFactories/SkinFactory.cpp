#include "SkinFactory.h"

#include "LetsGo/Utils/AssertUtils.h"

#define RETURN_IF_NULL(value) \
	if(value == nullptr) \
	{ \
		return;\
	} \

SkinFactory::SkinFactory(MaterialFactory* materialFactory, SkeletalMeshFactory* skeletalMeshFactory) :
_materialFactory(materialFactory),
_skeletalMeshFactory(skeletalMeshFactory)
{
	_skins.Add("Kachujin",
		new SkinData("Kachujin", "Kachujin")
	);
	_skins.Add("Maria",
		new SkinData("Maria", "Maria")
	);
	_skins.Add("Vampire",
		new SkinData("Vampire", "Vampire")
	);
}

void SkinFactory::SetSkin(AActor* actor, const FName skinId)
{
	AssertIsTrue(_skins.Contains(skinId));
	
	auto const skinData = _skins[skinId];
	auto const skeletalMeshId = skinData->SkeletalMeshId;
	auto const skeletalMesh = _skeletalMeshFactory->GetOrLoad(skeletalMeshId);
	RETURN_IF_NULL(skeletalMesh);

	auto const materialId = skinData->MaterialId;
	auto const material = _materialFactory->GetOrLoad(materialId);
	RETURN_IF_NULL(material);
	
	auto skeletalMeshComponent = actor->FindComponentByClass<USkeletalMeshComponent>();
	AssertIsNotNull(skeletalMeshComponent);
	
	skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
	skeletalMeshComponent->SetMaterial(0, material);
}
