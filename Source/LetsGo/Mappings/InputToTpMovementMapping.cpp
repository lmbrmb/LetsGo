#include "InputToTpMovementMapping.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Movement/ThirdPersonMovementComponent.h"

void UInputToTpMovementMapping::Map()
{
	auto const actor = GetOwner();

	auto const inputComponent = actor->InputComponent;
	if (!inputComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No input component", LogSeverity::Error);
		return;
	}

	const auto thirdPersonMovementComponent = actor->FindComponentByClass<UThirdPersonMovementComponent>();
	if (!thirdPersonMovementComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ", UThirdPersonMovementComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}
	
	inputComponent->BindAxis(InputConstant::AxisMoveHorizontal, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddActorRightMovementInput);
	inputComponent->BindAxis(InputConstant::AxisMoveVertical, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddActorForwardMovementInput);
	inputComponent->BindAxis(InputConstant::AxisLookHorizontal, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddSpringArmYawInput);
	inputComponent->BindAxis(InputConstant::AxisLookVertical, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddSpringArmPitchInput);
	inputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::Jump);
}
