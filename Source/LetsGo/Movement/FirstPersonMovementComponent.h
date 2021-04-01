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
	
	void AddActorForwardMovementInput(float amount);

	void AddActorRightMovementInput(float amount);

	void ProcessActorYaw(float deltaTime) const;

	void ProcessCameraPitch(float deltaTime) const;

	float ClampCameraPitch(float pitch) const;

	void AddActorYawInput(float amount);

	void AddCameraPitchInput(float amount);

	void StartSprint();

	void StopSprint();

protected:
	virtual void Init(AActor* actor) override;

	virtual void ProcessInput() override;
	
	virtual void ResetInput() override;

	virtual FVector GetMovementDirection() override;

	virtual float GetMovementSpeed() override;
	
	virtual void CustomTick(float deltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _sprintMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _airMultiplier = 0.25f;
	
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorYawSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchSpeed = 180.0f;

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

	bool _isSprinting = false;

	UCameraComponent* _cameraComponent = nullptr;

	UInputComponent* _playerInputComponent = nullptr;
};
