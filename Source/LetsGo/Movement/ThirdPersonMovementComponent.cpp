#include "ThirdPersonMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.1f;
const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;
const float SKIP_ROTATION_ANGLE_RADIANS = FMath::DegreesToRadians(1.0f);

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
	UpdateCache();
	ProcessSpringArmRotationTick(DeltaTime);
	ProcessActorLocationAndRotationTick(DeltaTime);
	ResetInput();
}

void UThirdPersonMovementComponent::Init(AActor* actor)
{
	_root = actor->GetRootComponent();
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
	_springArmComponent = actor->FindComponentByClass<USpringArmComponent>();
	_currentActorLocationCached = _root->GetComponentLocation();
	_previousActorLocationCached = _currentActorLocationCached;
	_actorTranslationVelocityCached = FVector::ZeroVector;
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

void UThirdPersonMovementComponent::AddActorRightMovementInput(const float amount)
{
	_actorRightMovementInputAmount = amount;
}

void UThirdPersonMovementComponent::AddActorForwardMovementInput(const float amount)
{
	_actorForwardMovementInputAmount = amount;
}

void UThirdPersonMovementComponent::AddSpringArmYawInput(const float amount)
{
	_springArmYawInput = amount;
}

void UThirdPersonMovementComponent::AddSpringArmPitchInput(const float amount)
{
	_springArmPitchInput = amount;
}

void UThirdPersonMovementComponent::ProcessSpringArmRotationTick(const float deltaTime) const
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

void UThirdPersonMovementComponent::ProcessActorLocationAndRotationTick(const float deltaTime) const
{
	auto const hasForwardInput = FMath::Abs(_actorForwardMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;
	auto const hasRightInput = FMath::Abs(_actorRightMovementInputAmount) > MIN_MOVEMENT_INPUT_AMOUNT;

	if(!hasForwardInput && !hasRightInput)
	{
		return;
	}

	FVector targetMovementDirection;
	if(hasForwardInput)
	{
		targetMovementDirection += _cameraComponent->GetForwardVector() * _actorForwardMovementInputAmount;
	}

	if (hasRightInput)
	{
		targetMovementDirection += _cameraComponent->GetRightVector() * _actorRightMovementInputAmount;
	}

	targetMovementDirection = FVector::VectorPlaneProject(targetMovementDirection, FVector::UpVector);
	targetMovementDirection.Normalize();
	
	DrawDebugLine(GetWorld(), _currentActorLocationCached, _currentActorLocationCached + targetMovementDirection * 100, FColor::Blue);
	
	auto actorForwardDirection = _root->GetForwardVector();
	actorForwardDirection = FVector::VectorPlaneProject(actorForwardDirection, FVector::UpVector);
	const auto targetDirectionDot = FVector::DotProduct(actorForwardDirection, targetMovementDirection);
	
	auto const targetAngleRadians = FMath::Acos(targetDirectionDot);

	if (targetAngleRadians > SKIP_ROTATION_ANGLE_RADIANS)
	{
		auto const targetAngleDegrees = FMath::RadiansToDegrees(targetAngleRadians);
		const auto targetDirectionCross = FVector::CrossProduct(actorForwardDirection, targetMovementDirection);
		auto const targetAngleSign = FMath::Sign(FVector::DotProduct(FVector::UpVector, targetDirectionCross));
		auto rotationDeltaDegrees = deltaTime * _rotationSpeedDegrees;
		
		if (rotationDeltaDegrees > targetAngleDegrees)
		{
			rotationDeltaDegrees = targetAngleDegrees;
		}

		auto const rotationSignedAngleDegrees = targetAngleSign * rotationDeltaDegrees;
		auto const rotationVector = actorForwardDirection.RotateAngleAxis(rotationSignedAngleDegrees, FVector::UpVector);
		auto const actorRotation = UKismetMathLibrary::MakeRotFromX(rotationVector);
		_root->SetWorldRotation(actorRotation);
	}

	if(targetDirectionDot > 0)
	{
		auto const actorMovementDelta = targetMovementDirection * targetDirectionDot * _movementSpeed * deltaTime;
		_root->AddWorldOffset(actorMovementDelta);
	}
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

void UThirdPersonMovementComponent::UpdateCache()
{
	_currentActorLocationCached = _root->GetComponentLocation();
	_actorTranslationVelocityCached = _currentActorLocationCached - _previousActorLocationCached;
	_previousActorLocationCached = _currentActorLocationCached;
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
