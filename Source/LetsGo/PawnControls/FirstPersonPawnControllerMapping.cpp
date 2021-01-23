#include "FirstPersonPawnControllerMapping.h"
#include "LetsGo/GenericMovementComponent.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/ProtagonistPawn.h"

FirstPersonPawnControllerMapping::FirstPersonPawnControllerMapping()
{
}

FirstPersonPawnControllerMapping::~FirstPersonPawnControllerMapping()
{
}

void FirstPersonPawnControllerMapping::Map(UInputComponent* playerInputComponent, APawn* pawn)
{
	_playerInputComponent = playerInputComponent;
	check(_playerInputComponent != nullptr);
	auto const genericMovementComponent = pawn->FindComponentByClass<UGenericMovementComponent>();
	check(genericMovementComponent);
	_playerInputComponent->BindAxis(InputConstant::AxisMoveHorizontal, genericMovementComponent, &UGenericMovementComponent::MoveInActorDirectionRight);
	_playerInputComponent->BindAxis(InputConstant::AxisMoveVertical, genericMovementComponent, &UGenericMovementComponent::MoveInActorDirectionUp);
	_playerInputComponent->BindAxis(InputConstant::AxisLookHorizontal, genericMovementComponent, &UGenericMovementComponent::RotateYaw);
	_playerInputComponent->BindAxis(InputConstant::AxisLookVertical, genericMovementComponent, &UGenericMovementComponent::RotatePitch);
	_playerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, genericMovementComponent, &UGenericMovementComponent::Jump);
}

void FirstPersonPawnControllerMapping::Unmap()
{
	TArray<FName> axisNames = {
		InputConstant::AxisMoveHorizontal,
		InputConstant::AxisMoveVertical,
		InputConstant::AxisLookHorizontal,
		InputConstant::AxisLookVertical
	};
	auto removePredicate = [axisNames](FInputAxisBinding binding) { return axisNames.Contains(binding.AxisName); };
	_playerInputComponent->AxisBindings.RemoveAll(removePredicate);
	_playerInputComponent->RemoveActionBinding(InputConstant::ActionJump, EInputEvent::IE_Pressed);
}

PawnControlScheme FirstPersonPawnControllerMapping::GetControlScheme()
{
	return Fps;
}
