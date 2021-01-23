#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GenericMovementComponent.generated.h"

///<summary>
/// Provides generic movement and rotation functions
///</summary>
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LETSGO_API UGenericMovementComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGenericMovementComponent();

	virtual void BeginPlay() override;

	void MoveInCameraDirectionUp(float amount);

	void MoveInCameraDirectionRight(float amount);

	void MoveInActorDirectionUp(float amount);

	void MoveInActorDirectionRight(float amount);

	void RotateYaw(float amount);

	void RotatePitch(float amount);

	void Jump();

private:
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float _moveForwardSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float _moveBackwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float _strafeSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	float _yawSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	float _pitchSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	float _pitchMin = -50.0f;

	UPROPERTY(EditAnywhere, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	float _pitchMax = 50.0f;

	UCameraComponent* _cameraComponent;

	void Move(FVector translationPerSecond);
};
