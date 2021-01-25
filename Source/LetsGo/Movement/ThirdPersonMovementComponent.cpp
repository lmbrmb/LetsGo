#include "ThirdPersonMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.1f;
const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;

UThirdPersonMovementComponent::UThirdPersonMovementComponent()
{
}

UThirdPersonMovementComponent::~UThirdPersonMovementComponent()
{
}

void UThirdPersonMovementComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessSpringArmRotationTick(DeltaTime);
	ProcessActorLocationAndRotationTick(DeltaTime);
	ResetInput();
}

void UThirdPersonMovementComponent::Init(AActor* actor)
{
	_root = actor->GetRootComponent();
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
	_springArmComponent = actor->FindComponentByClass<USpringArmComponent>();
}

void UThirdPersonMovementComponent::MapPlayerInput(UInputComponent* playerInputComponent)
{
	_playerInputComponent = playerInputComponent;
	_playerInputComponent->BindAxis(InputConstant::AxisMoveHorizontal, this, &UThirdPersonMovementComponent::AddActorRightMovementInput);
	_playerInputComponent->BindAxis(InputConstant::AxisMoveVertical, this, &UThirdPersonMovementComponent::AddActorForwardMovementInput);
	_playerInputComponent->BindAxis(InputConstant::AxisLookHorizontal, this, &UThirdPersonMovementComponent::AddSpringArmYawInput);
	_playerInputComponent->BindAxis(InputConstant::AxisLookVertical, this, &UThirdPersonMovementComponent::AddSpringArmPitchInput);
	_playerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, this, &UThirdPersonMovementComponent::Jump);
}

void UThirdPersonMovementComponent::AddActorRightMovementInput(float amount)
{
	_actorRightMovementInputAmount = amount;
}

void UThirdPersonMovementComponent::AddActorForwardMovementInput(float amount)
{
	_actorForwardMovementInputAmount = amount;
}

void UThirdPersonMovementComponent::AddSpringArmYawInput(float amount)
{
	_springArmYawInput = amount;
}

void UThirdPersonMovementComponent::AddSpringArmPitchInput(float amount)
{
	_springArmPitchInput = amount;
}

void UThirdPersonMovementComponent::ProcessSpringArmRotationTick(float deltaTime)
{
	auto const hasYawInput = FMath::Abs(_springArmYawInput) > MIN_ROTATION_INPUT_AMOUNT;
	auto const hasPitchInput = FMath::Abs(_springArmPitchInput) > MIN_ROTATION_INPUT_AMOUNT;
	
	if (!hasYawInput && !hasPitchInput)
	{
		return;
	}

	auto rotation = _springArmComponent->GetRelativeRotation();
	
	if(hasYawInput)
	{
		rotation.Yaw += _springArmYawInput * _springArmYawSpeed * deltaTime;
	}

	if(hasPitchInput)
	{
		rotation.Pitch = ClampSpringArmPitch(rotation.Pitch + _springArmPitchInput * _springArmPitchSpeed * deltaTime);
	}
	
	_springArmComponent->SetRelativeRotation(rotation);
}

void UThirdPersonMovementComponent::ProcessActorLocationAndRotationTick(float deltaTime)
{
	auto const hasForwardInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	auto const hasRightInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;

	if(!hasForwardInput && !hasRightInput)
	{
		return;
	}

	FVector cameraForwardDirection;
	if(hasForwardInput)
	{
		cameraForwardDirection += _cameraComponent->GetForwardVector() * _actorForwardMovementInputAmount;
	}

	if (hasRightInput)
	{
		cameraForwardDirection += _cameraComponent->GetRightVector() * _actorRightMovementInputAmount;
	}

	auto const actorLocation = _root->GetComponentLocation();
	cameraForwardDirection = FVector::VectorPlaneProject(cameraForwardDirection, FVector::UpVector);
	cameraForwardDirection.Normalize();
	auto const actorMovementDelta = cameraForwardDirection * _movementSpeed * deltaTime;
	
	auto const cameraRotation = _cameraComponent->GetComponentRotation();
	auto targetActorYaw = cameraRotation.Yaw;

	DrawDebugLine(GetWorld(), actorLocation, actorLocation + cameraForwardDirection * 100, FColor::Blue);
	
	if(hasForwardInput)
	{
		if(_actorForwardMovementInputAmount <0)
		{
			targetActorYaw += _actorForwardMovementInputAmount * 180;
			if(hasRightInput)
			{
				targetActorYaw -= _actorRightMovementInputAmount * 45;
			}
		}
		else
		{
			if (hasRightInput)
			{
				targetActorYaw += _actorRightMovementInputAmount * 45;
			}
		}
	}
	else
	{
		if (hasRightInput)
		{
			targetActorYaw += _actorRightMovementInputAmount * 90;
		}
	}

	auto actorRotation = _root->GetComponentRotation();
	actorRotation.Yaw = targetActorYaw;
	
	_root->SetWorldLocationAndRotation(actorLocation + actorMovementDelta, actorRotation);
}

float UThirdPersonMovementComponent::ClampSpringArmPitch(float pitch) const
{
	if (pitch > 0)
	{
		if (pitch > _springArmPitchMax)
		{
			pitch = _springArmPitchMax;
		}
	}
	else
	{
		if (pitch < _springArmPitchMin)
		{
			pitch = _springArmPitchMin;
		}
	}
	return pitch;
}

void UThirdPersonMovementComponent::Jump()
{
	DevLogger::GetLoggingChannel()->Log("Jump");
}

void UThirdPersonMovementComponent::ResetInput()
{
	_actorForwardMovementInputAmount = 0;
	_actorRightMovementInputAmount = 0;
	_springArmYawInput = 0;
	_springArmPitchInput = 0;
}
