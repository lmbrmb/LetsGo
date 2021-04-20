#pragma once

#include "CurveAnimationData.h"
#include "Components/ActorComponent.h"

#include "CurveAnimationBlenderComponent.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UCurveAnimationBlenderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCurveAnimationBlenderComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(USceneComponent* moveableComponent, USceneComponent* moveableComponentRoot);

	void AddAnimation(
		const FName& animationId,
		const bool loop,
		const bool isEnabled,
		float curveMagnitudeMultiplier,
		float curveTimeMultiplier
	);

	void AddCurve(const FName& animationId, UCurveFloat* curve, const FVector& direction);

	void ResetAnimationTime(const FName& animationId);

	void EnableAnimation(const FName& animationId, const bool isEnabled);

	const FName& GetBlenderId() const;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static const float DistanceErrorTolerance;

	static const float DistanceErrorToleranceSquared;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _blenderId;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _lerpSpeed = 0.25;

	USceneComponent* _moveableComponent = nullptr;

	USceneComponent* _moveableComponentRoot = nullptr;

	void ProcessAnimations(const float deltaTime);

	float SumAllCurves(TArray<UCurveFloat*>& curveArray, CurveAnimationData* curveAnimationData);

	float GetCurveValue(const UCurveFloat* curve, CurveAnimationData* curveAnimationData);

	float _bufferMinTime = 0;

	float _bufferMaxTime = 0;

	TMap<FName, CurveAnimationData*> _animations;

	float _previousForwardAmount = 0;

	float _previousRightAmount = 0;

	float _previousUpAmount = 0;
};
