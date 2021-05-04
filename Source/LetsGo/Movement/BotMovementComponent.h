#pragma once

#include "MovementComponentBase.h"

#include "RotationTarget.h"

#include "BotMovementComponent.generated.h"

/// <summary>
/// Bot movement component
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UBotMovementComponent final : public UMovementComponentBase
{
	GENERATED_BODY()

public:
	const FVector& GetTargetLocation() const;

	bool IsTargetLocationValid() const;

	void SetTargetLocation(const FVector& targetLocation);

	void ClearTargetLocation();

	const FVector& GetTargetRotation() const;

	void SetTargetRotationAsLocation(const FVector& targetRotationLocation);

	void SetTargetRotationAsMovementDirection();

	void ClearTargetRotation();

protected:
	virtual const FVector& GetMovementDirection() const override;

	virtual float GetBaseMovementSpeed() override;

	virtual float GetAbsoluteMovementAmount() const override;

	virtual void Init(AActor* actor) override;

	virtual void ProcessInput() override;

	virtual void ResetInput() override;

	virtual void CustomTick(const float deltaTime) override;

private:
	const float MIN_DOT_FORWARD = -0.01f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	bool _isTargetLocationValid = false;

	RotationTarget _rotationTarget = RotationTarget::None;

	FVector _targetLocation = FVector::ZeroVector;

	FVector _targetRotationLocation = FVector::ZeroVector;

	FVector _inputMovementDirection = FVector::ZeroVector;

	void ProcessTargetRotation(const float deltaTime) const;

	void CalculateNextLocation();

	FVector _nextLocation;

	bool _isNextLocationValid;
};
