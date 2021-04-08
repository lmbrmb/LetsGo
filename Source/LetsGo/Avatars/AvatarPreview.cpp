#include "AvatarPreview.h"

#include "LetsGo/GameModes/ProjectGameModeBase.h"

AAvatarPreview::AAvatarPreview()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAvatarPreview::BeginPlay()
{
	Super::BeginPlay();

	_skeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();
	AssertIsNotNull(_skeletalMeshComponent);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AProjectGameModeBase>(authGameMode);
	auto const diContainer = matchGameMode->GetDiContainer();

	auto const skinFactory = diContainer->GetInstance<SkinFactory>();
	_skinFactory = &skinFactory.Get();
}

// This function should not be const, otherwise there will be no pin in blueprint to execute it
// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void AAvatarPreview::SetSkin(const FName& skinId)
{
	_skinFactory->SetSkin(_skeletalMeshComponent, skinId);
}
