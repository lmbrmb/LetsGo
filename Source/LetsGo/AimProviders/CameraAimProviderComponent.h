#pragma once

#include "AimProviderComponent.h"
#include "Camera/CameraComponent.h"


#include "CameraAimProviderComponent.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UCameraAimProviderComponent : public UAimProviderComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	void SetCamera(UCameraComponent* cameraComponent);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _aimRange = 2000.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TEnumAsByte<ECollisionChannel> _collisionChannel;
};
