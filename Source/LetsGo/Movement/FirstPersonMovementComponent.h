#pragma once

#include "CoreMinimal.h"
#include "MovementComponentBase.h"
#include "Camera/CameraComponent.h"
#include "LetsGo/Force.h"
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

	FVector GetInputMovementAmount(float deltaTime);

	void ProcessForcesTick(float deltaTime);
	
	void ProcessActorYawTick(float deltaTime) const;

	void CheckGround();

	void AddMovement(FVector inputMovementAmount);
	
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
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceMagnitude = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceDuration = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	int _jumpCount = 1;

	int _jumpIndex = 0;
	
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _gravityForceMagnitude = 981.0f;
	
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
	
	float _actorForwardMovementInputAmount = 0;

	float _actorRightMovementInputAmount = 0;

	float _actorYawInputAmount = 0;

	float _cameraPitchInputAmount = 0;

	float _movementAmount;

	bool _isSprinting = false;

	bool _isInAir = false;
	
	FCollisionQueryParams _collisionQueryParams;
	
	UWorld* _world = nullptr;
	
	USceneComponent* _root = nullptr;

	UShapeComponent* _rootCollider = nullptr;

	FCollisionShape _collisionShape;
	
	UCameraComponent* _cameraComponent = nullptr;
	
	UInputComponent* _playerInputComponent = nullptr;

	TArray<Force*> _forces;
	
	UFUNCTION(BlueprintCallable)
	float GetMovementAmount();

	void ResetInput();

	FHitResult _floorHitResult;
	
	FHitResult _hitResult;

	FVector _previousLocation = FVector::ZeroVector;

	FVector _velocity = FVector::ZeroVector;
};
