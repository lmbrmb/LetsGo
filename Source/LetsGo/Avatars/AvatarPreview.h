#pragma once

#include "GameFramework/Actor.h"
#include "LetsGo/AssetFactories/SkinFactory.h"

#include "AvatarPreview.generated.h"

UCLASS()
class LETSGO_API AAvatarPreview : public AActor
{
	GENERATED_BODY()

public:	
	AAvatarPreview();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetSkin(const FName& skinId);

private:
	SkinFactory* _skinFactory;

	USkeletalMeshComponent* _skeletalMeshComponent;
};
