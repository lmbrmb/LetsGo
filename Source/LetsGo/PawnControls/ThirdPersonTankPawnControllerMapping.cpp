#include "ThirdPersonTankPawnControllerMapping.h"
#include "LetsGo/GenericMovementComponent.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/ProtagonistPawn.h"

ThirdPersonTankPawnControllerMapping::ThirdPersonTankPawnControllerMapping()
{
}

ThirdPersonTankPawnControllerMapping::~ThirdPersonTankPawnControllerMapping()
{
}

void ThirdPersonTankPawnControllerMapping::Map(UInputComponent* playerInputComponent, APawn* pawn)
{
	_playerInputComponent = playerInputComponent;
	check(_playerInputComponent != nullptr);
	auto const genericMovementComponent = pawn->FindComponentByClass<UGenericMovementComponent>();
	check(genericMovementComponent);
	_playerInputComponent->BindAxis(InputConstant::AxisMoveHorizontal, genericMovementComponent, &UGenericMovementComponent::RotateYaw);
	_playerInputComponent->BindAxis(InputConstant::AxisMoveVertical, genericMovementComponent, &UGenericMovementComponent::MoveInActorDirectionUp);
	_playerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, genericMovementComponent, &UGenericMovementComponent::Jump);
}

void ThirdPersonTankPawnControllerMapping::Unmap()
{
	TArray<FName> axisNames = { InputConstant::AxisMoveHorizontal, InputConstant::AxisMoveVertical };
	auto removePredicate = [axisNames](FInputAxisBinding binding) { return axisNames.Contains(binding.AxisName); };
	_playerInputComponent->AxisBindings.RemoveAll(removePredicate);
	_playerInputComponent->RemoveActionBinding(InputConstant::ActionJump, EInputEvent::IE_Pressed);
}

PawnControlScheme ThirdPersonTankPawnControllerMapping::GetControlScheme()
{
	return ThirdPersonTank;
}
