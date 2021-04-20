#pragma once

#include "MappingComponent.h"

#include "LetsGo/Movement/MovementComponentBase.h"
#include "LetsGo/Animations/CurveAnimationBlenderComponent.h"

#include "GunRunAnimationMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UGunRunAnimationMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:
	UGunRunAnimationMapping();

protected:
	virtual void Map() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _blenderId;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _animationId;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	bool _isAnimationEnabled = false;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	bool _animationLoop = true;

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

	UMovementComponentBase* _movementComponent = nullptr;

	UCurveAnimationBlenderComponent* _curveAnimationBlenderComponent = nullptr;

	bool _isInitialized = false;

	bool _isEnabled = false;

	void SetupAnimation() const;
};
