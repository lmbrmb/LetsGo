#include "FirstPersonPawnControllerMapping.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/ProtagonistPawn.h"

const float MIN_AXIS_INPUT_ABS = 0.1f;

FirstPersonPawnControllerMapping::FirstPersonPawnControllerMapping()
{
}

FirstPersonPawnControllerMapping::~FirstPersonPawnControllerMapping()
{
}

void FirstPersonPawnControllerMapping::Map(UInputComponent* playerInputComponent, APawn* pawn)
{
	_protagonistPawn = Cast<AProtagonistPawn>(pawn);
	check(_protagonistPawn != nullptr);
	_playerInputComponent = playerInputComponent;
	check(_playerInputComponent != nullptr);
	
	_playerInputComponent->BindAxis(InputConstant::AxisHorizontal, _protagonistPawn, &AProtagonistPawn::MoveRight);
	_playerInputComponent->BindAxis(InputConstant::AxisVertical, _protagonistPawn, &AProtagonistPawn::MoveForward);
	_playerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, _protagonistPawn, &AProtagonistPawn::Jump);
}

void FirstPersonPawnControllerMapping::Unmap()
{
	TArray<FName> axisNames = { InputConstant::AxisHorizontal, InputConstant::AxisVertical };
	auto removePredicate = [axisNames](FInputAxisBinding binding) { return axisNames.Contains(binding.AxisName); };
	_playerInputComponent->AxisBindings.RemoveAll(removePredicate);
	_playerInputComponent->RemoveActionBinding(InputConstant::ActionJump, EInputEvent::IE_Pressed);
}

FString FirstPersonPawnControllerMapping::GetName()
{
	return "FPS";
}
