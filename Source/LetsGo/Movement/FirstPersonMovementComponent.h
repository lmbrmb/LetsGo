#pragma once

#include "CoreMinimal.h"
#include "MovementComponentBase.h"
#include "Camera/CameraComponent.h"
#include "FirstPersonMovementComponent.generated.h"

UCLASS(EditInlineNew, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LETSGO_API UFirstPersonMovementComponent final : public UMovementComponentBase
{
	GENERATED_BODY()

public:
	UFirstPersonMovementComponent();

	~UFirstPersonMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddActorForwardMovementInput(float amount);

	void AddActorRightMovementInput(float amount);

	void ProcessActorMovementTick(float deltaTime);

	void ProcessActorYawTick(float deltaTime);

	void ProcessCameraPitchTick(float deltaTime) const;

	float ClampCameraPitch(float pitch) const;

	void AddActorYawInput(float amount);

	void AddCameraPitchInput(float amount);

	void Jump();

	void StartSprint();

	void StopSprint();
	
protected:
	virtual void Init(AActor* actor) override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _sprintMultiplier = 1.5f;
	
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorYawSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchMin = -70.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchMax = 70.0f;

	UFUNCTION(BlueprintCallable)
	float GetMovementAmount();
	
	float _actorForwardMovementInputAmount = 0;

	float _actorRightMovementInputAmount = 0;

	float _actorYawInputAmount = 0;

	float _cameraPitchInputAmount = 0;

	float _movementAmount;
	
	USceneComponent* _root = nullptr;

	UCameraComponent* _cameraComponent = nullptr;
	
	UInputComponent* _playerInputComponent = nullptr;

	bool _isSprinting = false;

	void ResetInput();
};
