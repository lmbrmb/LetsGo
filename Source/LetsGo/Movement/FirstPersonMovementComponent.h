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
	
protected:
	virtual void Init(AActor* actor) override;
	
	virtual void MapPlayerInput(UInputComponent* playerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorYawSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchMin = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _cameraPitchMax = 50.0f;

	float _actorForwardMovementInputAmount = 0;

	float _actorRightMovementInputAmount = 0;

	float _actorYawInputAmount = 0;

	float _cameraPitchInputAmount = 0;
	
	USceneComponent* _root = nullptr;

	UCameraComponent* _cameraComponent = nullptr;
	
	UInputComponent* _playerInputComponent = nullptr;
	
	void AddActorForwardMovementInput(float amount);

	void AddActorRightMovementInput(float amount);

	void ProcessActorMovementTick(float deltaTime, OUT FVector& movementDelta, OUT bool& isMoved ) const;
	
	void ProcessActorYawTick(float deltaTime, OUT float& yawDelta, OUT bool& isRotated) const;

	void ProcessCameraPitchTick(float deltaTime) const;
	
	float ClampCameraPitch(float pitch) const;
	
	void AddActorYawInput(float amount);

	void AddCameraPitchInput(float amount);

	void ResetInput();
	
	void Jump();
};
