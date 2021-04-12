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
	virtual float GetAbsoluteMovementAmount() const override;

	void AddActorForwardMovementInput(const float amount);

	void AddActorRightMovementInput(const float amount);

	void AddSpringArmYawInput(const float amount);

	void AddSpringArmPitchInput(const float amount);

	void ProcessSpringArmRotation(const float deltaTime) const;
	
protected:
	virtual void Init(AActor* actor) override;

	virtual void ProcessInput() override;
	
	virtual void ResetInput() override;

	virtual const FVector& GetMovementDirection() const override;

	virtual float GetBaseMovementSpeed() override;
	
	virtual void CustomTick(const float deltaTime) override;
	
private:
	const float MIN_MOVEMENT_INPUT_AMOUNT = 0.15f;

	const float MIN_ROTATION_INPUT = 0.05f;

	const float MIN_MOVEMENT_DOT = 0.25f;

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
