#include "CurveAnimationBlenderComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/AssertUtils.h"

const float UCurveAnimationBlenderComponent::DistanceErrorTolerance = 0.1f;

const float UCurveAnimationBlenderComponent::DistanceErrorToleranceSquared = DistanceErrorTolerance * DistanceErrorTolerance;

UCurveAnimationBlenderComponent::UCurveAnimationBlenderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCurveAnimationBlenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!_moveableComponent || !_moveableComponentRoot)
	{
		return;
	}

	ProcessAnimations(DeltaTime);
}

void UCurveAnimationBlenderComponent::Initialize(USceneComponent* moveableComponent, USceneComponent* moveableComponentRoot)
{
	_moveableComponent = moveableComponent;
	_moveableComponentRoot = moveableComponentRoot;
}

void UCurveAnimationBlenderComponent::AddAnimation(
	const FName& animationId, 
	const bool loop,
	const bool isEnabled,
	float curveMagnitudeMultiplier,
	float curveTimeMultiplier
)
{
	AssertContainerDoesNotContainElement(_animations, animationId);
	
	auto const animation = new CurveAnimationData();
	animation->Loop = loop;
	animation->IsEnabled = isEnabled;
	animation->CurveMagnitudeMultiplier = curveMagnitudeMultiplier;
	animation->CurveTimeMultiplier = curveTimeMultiplier;
	
	_animations.Add(animationId, animation);
}

void UCurveAnimationBlenderComponent::AddCurve(const FName& animationId, UCurveFloat* curve, const FVector& direction)
{
	AssertIsNotNull(curve);
	AssertContainerContainsElement(_animations, animationId);
	
	auto animation = _animations[animationId];
	
	if(direction == FVector::UpVector)
	{
		animation->UpCurves.Add(curve);
	}
	else if(direction == FVector::RightVector)
	{
		animation->RightCurves.Add(curve);
	}
	else if (direction == FVector::ForwardVector)
	{
		animation->ForwardCurves.Add(curve);
	}
}

void UCurveAnimationBlenderComponent::ResetAnimationTime(const FName& animationId)
{
	AssertContainerContainsElement(_animations, animationId);
	auto const animation = _animations[animationId];
	animation->Time = 0;
}

void UCurveAnimationBlenderComponent::EnableAnimation(const FName& animationId, const bool isEnabled)
{
	AssertContainerContainsElement(_animations, animationId);
	auto const animation = _animations[animationId];
	animation->IsEnabled = isEnabled;
}

const FName& UCurveAnimationBlenderComponent::GetBlenderId() const
{
	return _blenderId;
}

void UCurveAnimationBlenderComponent::ProcessAnimations(const float deltaTime)
{
	if(_animations.Num() <= 0)
	{
		return;
	}

	auto forwardRaw = 0.0f;
	auto rightRaw = 0.0f;
	auto upRaw = 0.0f;
	
	for (const auto animationRecord : _animations)
	{
		auto const animation = animationRecord.Value;
		
		if(!animation->IsEnabled)
		{
			continue;
		}
		
		forwardRaw += SumAllCurves(animation->ForwardCurves, animation);
		rightRaw += SumAllCurves(animation->RightCurves, animation);
		upRaw += SumAllCurves(animation->UpCurves, animation);
		animation->Time += deltaTime;
	}

	auto const rootLocation = _moveableComponentRoot->GetComponentLocation();

	auto const forwardAmount = FMath::Lerp(_previousForwardAmount, forwardRaw, _lerpSpeed);
	auto const rightAmount = FMath::Lerp(_previousRightAmount, rightRaw, _lerpSpeed);
	auto const upAmount = FMath::Lerp(_previousUpAmount, upRaw, _lerpSpeed);
	
	auto const offset =
		_moveableComponentRoot->GetForwardVector() * forwardAmount +
		_moveableComponentRoot->GetRightVector() * rightAmount +
		_moveableComponentRoot->GetUpVector() * upAmount;

	auto const targetLocation = rootLocation + offset;
	
	_moveableComponent->SetWorldLocation(targetLocation);
	_previousForwardAmount = forwardAmount;
	_previousRightAmount = rightAmount;
	_previousUpAmount = upAmount;
}

float UCurveAnimationBlenderComponent::SumAllCurves(TArray<UCurveFloat*>& curveArray, CurveAnimationData* curveAnimationData)
{
	auto sum = 0.0f;
	for (auto const curve : curveArray)
	{
		auto const value = GetCurveValue(curve, curveAnimationData);
		sum += value;
	}
	return sum;
}

float UCurveAnimationBlenderComponent::GetCurveValue(const UCurveFloat* curve, CurveAnimationData* curveAnimationData)
{
	if(!curve)
	{
		return 0;
	}
	
	curve->GetTimeRange(_bufferMinTime, _bufferMaxTime);

	auto const scaledTime = curveAnimationData->Time * curveAnimationData->CurveTimeMultiplier;
	auto const timeValue = scaledTime <= _bufferMaxTime ? scaledTime :
		curveAnimationData->Loop ? FMath::Fmod(scaledTime, _bufferMaxTime) : _bufferMaxTime;
	auto const curveValue = curve->GetFloatValue(timeValue);
	auto const offset = curveValue * curveAnimationData->CurveMagnitudeMultiplier;
	
	return offset;
}
