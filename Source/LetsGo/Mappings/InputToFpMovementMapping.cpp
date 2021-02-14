#include "InputToFpMovementMapping.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Movement/FirstPersonMovementComponent.h"

void UInputToFpMovementMapping::Map()
{
	auto const actor = GetOwner();

	auto const inputComponent = actor->InputComponent;
	if (!inputComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No input component", LogSeverity::Error);
		return;
	}

	const auto firstPersonMovementComponent = actor->FindComponentByClass<UFirstPersonMovementComponent>();
	if (!firstPersonMovementComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ", UFirstPersonMovementComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}

	inputComponent->BindAxis(InputConstant::AxisMoveHorizontal, firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddActorRightMovementInput);
	inputComponent->BindAxis(InputConstant::AxisMoveVertical, firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddActorForwardMovementInput);
	inputComponent->BindAxis(InputConstant::AxisLookHorizontal, firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddActorYawInput);
	inputComponent->BindAxis(InputConstant::AxisLookVertical, firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddCameraPitchInput);
	inputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed,
		firstPersonMovementComponent, &UFirstPersonMovementComponent::Jump);
	inputComponent->BindAction(InputConstant::ActionSprint, EInputEvent::IE_Pressed,
		firstPersonMovementComponent, &UFirstPersonMovementComponent::StartSprint);
	inputComponent->BindAction(InputConstant::ActionSprint, EInputEvent::IE_Released,
		firstPersonMovementComponent, &UFirstPersonMovementComponent::StopSprint);
}
