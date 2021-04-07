#include "InputToTpMovementMapping.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Movement/ThirdPersonMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UInputToTpMovementMapping::Map()
{
	auto const actor = GetOwner();
	auto const inputComponent = actor->InputComponent;
	
	AssertIsNotNull(inputComponent);

	const auto thirdPersonMovementComponent = actor->FindComponentByClass<UThirdPersonMovementComponent>();

	AssertIsNotNull(thirdPersonMovementComponent);
	
	inputComponent->BindAxis(InputConstant::AxisMoveHorizontal, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddActorRightMovementInput);
	inputComponent->BindAxis(InputConstant::AxisMoveVertical, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddActorForwardMovementInput);
	inputComponent->BindAxis(InputConstant::AxisLookHorizontal, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddSpringArmYawInput);
	inputComponent->BindAxis(InputConstant::AxisLookVertical, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddSpringArmPitchInput);
	inputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::PerformJump);
	StartDestroyTask();
}
