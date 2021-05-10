#include "SkinFactory.h"

#include "LetsGo/Utils/AssertUtils.h"

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

SkinFactory::~SkinFactory()
{
	delete _materialFactory;
	delete _skeletalMeshFactory;
}

void SkinFactory::SetSkin(AActor* actor, const FName& skinId)
{
	AssertIsNotNull(actor);
	auto const skeletalMeshComponent = actor->FindComponentByClass<USkeletalMeshComponent>();
	SetSkin(skeletalMeshComponent, skinId);
}

void SkinFactory::SetSkin(USkeletalMeshComponent* skeletalMeshComponent, const FName& skinId)
{
	AssertIsNotNull(skeletalMeshComponent);
	AssertContainerContainsElement(_skins, skinId);

	auto const skinData = _skins[skinId];
	auto const skeletalMeshId = skinData->SkeletalMeshId;
	auto const skeletalMesh = _skeletalMeshFactory->GetOrLoad(skeletalMeshId);
	AssertIsNotNull(skeletalMesh);

	auto const materialId = skinData->MaterialId;
	auto const material = _materialFactory->GetOrLoad(materialId);
	AssertIsNotNull(material);

	skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
	skeletalMeshComponent->SetMaterial(0, material);
}
