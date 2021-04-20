#pragma once

#include "CurveAnimationState.h"
#include "Components/ActorComponent.h"

#include "CurveAnimationComponent.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UCurveAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCurveAnimationComponent();
	
	UFUNCTION(BlueprintCallable)
	void Initialize(USceneComponent* moveableComponent, USceneComponent* moveableComponentRoot);

	void SetState(const FCurveAnimationState state);

	const FName& GetAnimationId() const;
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _curveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _curveTimeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _moveToOriginLerpSpeed = 5;
	
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _curveForward;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _curveRight;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _curveUp;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	bool _loopTime = false;

	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _animationId;

	FCurveAnimationState _state;
	
	USceneComponent* _moveableComponent = nullptr;
	
	USceneComponent* _moveableComponentRoot = nullptr;

	void ProcessApplyCurveState(const float deltaTime);

	void ProcessMoveToOriginState(const float deltaTime) const;
	
	FVector GetLocationOffset(const FVector& direction, const UCurveFloat* curve);
	
	float GetOffsetAmount(const UCurveFloat* curve);
	
	float _stateTime;

	float _bufferMinTime;
	
	float _bufferMaxTime;

	static const float DistanceErrorTolerance;
		
	static const float DistanceErrorToleranceSquared;
};
