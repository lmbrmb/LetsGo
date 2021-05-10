#pragma once

#include "MovementComponentBase.h"
#include "Camera/CameraComponent.h"

#include "FirstPersonMovementComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UFirstPersonMovementComponent final : public UMovementComponentBase
{
	GENERATED_BODY()

public:
	virtual float GetAbsoluteMovementAmount() const override;

	void SetLookSpeed(
		float cameraPitchSpeed,
		float actorYawSpeed
	);

	void AddActorForwardMovementInput(float amount);

	void AddActorRightMovementInput(float amount);

	void AddActorYawInput(float amount);

	void AddCameraPitchInput(float amount);

protected:
	virtual void Init(AActor* actor) override;

	virtual void ProcessInput() override;

	virtual void ResetInput() override;

	virtual const FVector& GetMovementDirection() const override;

	virtual float GetBaseMovementSpeed() override;

	virtual void CustomTick(const float deltaTime) override;

private:
	const float MIN_DOT_FORWARD = -0.01f;

	const float MIN_MOVEMENT_INPUT = 0.1f;

	const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorYawSpeed = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchSpeed = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchMin = -70.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchMax = 70.0f;

	float _actorForwardMovementInputAmount = 0;

	float _actorRightMovementInputAmount = 0;

	float _actorYawInputAmount = 0;

	float _cameraPitchInputAmount = 0;

	FVector _inputMovementDirection = FVector::ZeroVector;

	float _absoluteMovementAmount = 0;

	UCameraComponent* _cameraComponent = nullptr;

	UInputComponent* _playerInputComponent = nullptr;

	void ProcessCameraPitch(const float deltaTime) const;

	void ProcessActorYaw(const float deltaTime) const;

	float ClampCameraPitch(float pitch) const;
};
