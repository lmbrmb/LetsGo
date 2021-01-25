#pragma once

#include "CoreMinimal.h"
#include "MovementComponentBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "ThirdPersonMovementComponent.generated.h"

UCLASS(EditInlineNew, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LETSGO_API UThirdPersonMovementComponent final : public UMovementComponentBase
{
	GENERATED_BODY()

public:
	UThirdPersonMovementComponent();

	virtual ~UThirdPersonMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void Init(AActor* actor) override;
	
	virtual void MapPlayerInput(UInputComponent* playerInputComponent) override;

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
	
	USceneComponent* _root = nullptr;

	USpringArmComponent* _springArmComponent = nullptr;

	UCameraComponent* _cameraComponent = nullptr;
	
	UInputComponent* _playerInputComponent = nullptr;

	void AddActorForwardMovementInput(float amount);

	void AddActorRightMovementInput(float amount);

	void AddSpringArmYawInput(float amount);

	void AddSpringArmPitchInput(float amount);

	void ProcessSpringArmRotationTick(float deltaTime);
	
	void ProcessActorLocationAndRotationTick(float deltaTime);
	
	void Jump();

	void ResetInput();

	float ClampSpringArmPitch(float pitch) const;
	
	float _actorForwardMovementInputAmount = 0;
	
	float _actorRightMovementInputAmount = 0;

	float _springArmYawInput = 0;

	float _springArmPitchInput = 0;
};
