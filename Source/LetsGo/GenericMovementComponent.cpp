#include "GenericMovementComponent.h"
#include "Logs/DevLogger.h"
#include "Kismet/KismetStringLibrary.h"

const float MIN_MOVEMENT_INPUT_AMOUNT = 0.1f;
const float MIN_ROTATION_INPUT_AMOUNT = 0.05f;

#define RETURN_IF_MIN_INPUT(value, minValue) \
	if(FMath::Abs(value) < minValue) \
	{return;}

UGenericMovementComponent::UGenericMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGenericMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	auto actor = GetAttachmentRootActor();
	// TODO: assert not null
	_cameraComponent = actor->FindComponentByClass<UCameraComponent>();
}

void UGenericMovementComponent::MoveInCameraDirectionUp(float amount)
{
}

void UGenericMovementComponent::MoveInCameraDirectionRight(float amount)
{
}

void UGenericMovementComponent::MoveInActorDirectionUp(float amount)
{
	RETURN_IF_MIN_INPUT(amount, MIN_MOVEMENT_INPUT_AMOUNT);
	auto const root = GetAttachmentRoot();
	const auto forwardVector = root->GetForwardVector();
	auto const speed = amount < 0 ? _moveBackwardSpeed : _moveForwardSpeed;
	auto const translationPerSecond = forwardVector * speed * amount;
	Move(translationPerSecond);
}

void UGenericMovementComponent::MoveInActorDirectionRight(float amount)
{
	RETURN_IF_MIN_INPUT(amount, MIN_MOVEMENT_INPUT_AMOUNT);
	auto const root = GetAttachmentRoot();
	const auto rightVector = root->GetRightVector();
	auto const speed = _strafeSpeed;
	auto const translationPerSecond = rightVector * speed * amount;
	Move(translationPerSecond);
}

void UGenericMovementComponent::Move(FVector translationPerSecond)
{
	const auto deltaTime = GetWorld()->GetDeltaSeconds();
	auto const translationDelta = translationPerSecond * deltaTime;
	auto const root = GetAttachmentRoot();
	root->MoveComponent(translationDelta, root->GetComponentQuat(), true, nullptr, MOVECOMP_NoFlags, ETeleportType::None);
}

void UGenericMovementComponent::RotateYaw(float amount)
{
	RETURN_IF_MIN_INPUT(amount, MIN_ROTATION_INPUT_AMOUNT);
	const auto deltaTime = GetWorld()->GetDeltaSeconds();
	auto const rotationDelta = _yawSpeed * amount * deltaTime;
	auto const root = GetAttachmentRoot();
	const FRotator rotator(0, rotationDelta, 0);
	root->AddLocalRotation(rotator);
}

void UGenericMovementComponent::RotatePitch(float amount)
{
	RETURN_IF_MIN_INPUT(amount, MIN_ROTATION_INPUT_AMOUNT);
	auto rotation = _cameraComponent->GetRelativeRotation();
	const auto deltaTime = GetWorld()->GetDeltaSeconds();
	auto newPitch = rotation.Pitch + amount * _pitchSpeed * deltaTime;
	if (newPitch > 0)
	{
		if(newPitch > _pitchMax)
		{
			newPitch = _pitchMax;
		}
	}
	else
	{
		if (newPitch < _pitchMin)
		{
			newPitch = _pitchMin;
		}
	}
	rotation.Pitch = newPitch;
	
	_cameraComponent->SetRelativeRotation(rotation);
}

void UGenericMovementComponent::Jump()
{
	DevLogger::GetLoggingChannel()->Log("Jump");
}
