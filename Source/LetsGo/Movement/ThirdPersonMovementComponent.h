#pragma once

#include "MovementComponentBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "ThirdPersonMovementComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UThirdPersonMovementComponent final : public UMovementComponentBase
{
	GENERATED_BODY()

public:
	/// <summary>
	/// IMovementInfoProvider.GetAbsoluteMovementAmount implementation
	/// </summary>
	virtual float GetAbsoluteMovementAmount() override;

	void AddActorForwardMovementInput(const float amount);

	void AddActorRightMovementInput(const float amount);

	void AddSpringArmYawInput(const float amount);

	void AddSpringArmPitchInput(const float amount);

	void ProcessSpringArmRotation(const float deltaTime) const;

	void ProcessActorRotation(const float deltaTime) const;
	
protected:
	virtual void Init(AActor* actor) override;

	virtual void ProcessInput() override;
	
	virtual void ResetInput() override;

	virtual FVector GetInputMovementDirection() override;

	virtual float GetMovementSpeed() override;
	
	virtual void CustomTick(float deltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _movementSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _springArmYawSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _springArmPitchSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _springArmPitchMin = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _springArmPitchMax = 50.0f;
	
	/// <summary>
	/// Rotation speed in degrees per second
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _rotationSpeedDegrees = 540.0f;
	
	USpringArmComponent* _springArmComponent = nullptr;

	UCameraComponent* _cameraComponent = nullptr;

	float ClampSpringArmPitch(float pitch) const;

	float _absoluteMovementAmount = 0;
	
	float _actorForwardMovementInputAmount = 0;
	
	float _actorRightMovementInputAmount = 0;

	float _springArmYawInput = 0;

	float _springArmPitchInput = 0;

	float _maxSquaredMovementSpeed = 0;
	
	float _normalizedMovementSpeed = 0;
	
	FVector _inputMovementDirection = FVector::ZeroVector;
};
