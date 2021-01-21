#include "ThirdPersonTankPawnControllerMapping.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/ProtagonistPawn.h"

const float MIN_AXIS_INPUT_ABS = 0.1f;

ThirdPersonTankPawnControllerMapping::ThirdPersonTankPawnControllerMapping()
{
}

ThirdPersonTankPawnControllerMapping::~ThirdPersonTankPawnControllerMapping()
{
}

void ThirdPersonTankPawnControllerMapping::Map(UInputComponent* playerInputComponent, APawn* pawn)
{
	_protagonistPawn = Cast<AProtagonistPawn>(pawn);
	check(_protagonistPawn != nullptr);
	_playerInputComponent = playerInputComponent;
	check(_playerInputComponent != nullptr);
	
	_playerInputComponent->BindAxis(InputConstant::AxisHorizontal, _protagonistPawn, &AProtagonistPawn::RotateRight);
	_playerInputComponent->BindAxis(InputConstant::AxisVertical, _protagonistPawn, &AProtagonistPawn::MoveForward);
	_playerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, _protagonistPawn, &AProtagonistPawn::Jump);
}

void ThirdPersonTankPawnControllerMapping::Unmap()
{
	TArray<FName> axisNames = { InputConstant::AxisHorizontal, InputConstant::AxisVertical };
	auto removePredicate = [axisNames](FInputAxisBinding binding) { return axisNames.Contains(binding.AxisName); };
	_playerInputComponent->AxisBindings.RemoveAll(removePredicate);
	_playerInputComponent->RemoveActionBinding(InputConstant::ActionJump, EInputEvent::IE_Pressed);
}

FString ThirdPersonTankPawnControllerMapping::GetName()
{
	return "TankControls";
}
