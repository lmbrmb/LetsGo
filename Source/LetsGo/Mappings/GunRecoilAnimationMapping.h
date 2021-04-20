#pragma once

#include "MappingComponent.h"

#include "LetsGo/Animations/CurveAnimationBlenderComponent.h"

#include "GunRecoilAnimationMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UGunRecoilAnimationMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:
	UGunRecoilAnimationMapping();

protected:
	virtual void Map() override;
	
private:
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _blenderId;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _animationId;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	bool _isAnimationEnabled = false;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	bool _animationLoop = false;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _curveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _curveTimeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _curveForward;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _curveRight;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _curveUp;

	UCurveAnimationBlenderComponent* _curveAnimationBlenderComponent = nullptr;

	bool _isInitialized = false;

	void SetupAnimation() const;

	void OnShotRequested() const;
};
