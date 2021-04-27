#include "InputToTpMovementMapping.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Movement/ThirdPersonMovementComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UInputToTpMovementMapping::Map()
{
	auto const owner = GetOwner();
	_inputComponent = owner->InputComponent;
	AssertIsNotNull(_inputComponent);

	_thirdPersonMovementComponent = owner->FindComponentByClass<UThirdPersonMovementComponent>();
	AssertIsNotNull(_thirdPersonMovementComponent);

	Bind();

	auto const healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	healthComponent->Died.AddUObject(this, &UInputToTpMovementMapping::OnOwnerDied);
}

void UInputToTpMovementMapping::OnOwnerDied(UHealthComponent* healthComponent, float delta) const
{
	Unbind();
}

void UInputToTpMovementMapping::Bind()
{
	_inputComponent->BindAxis(InputConstant::AxisMoveHorizontal, _thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddActorRightMovementInput);
	_inputComponent->BindAxis(InputConstant::AxisMoveVertical, _thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddActorForwardMovementInput);
	_inputComponent->BindAxis(InputConstant::AxisLookHorizontal, _thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddSpringArmYawInput);
	_inputComponent->BindAxis(InputConstant::AxisLookVertical, _thirdPersonMovementComponent,
		&UThirdPersonMovementComponent::AddSpringArmPitchInput);
	
	_inputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed,
		_thirdPersonMovementComponent, &UThirdPersonMovementComponent::PerformJump);
	_inputComponent->BindAction(InputConstant::ActionSprint, EInputEvent::IE_Pressed,
		this, &UInputToTpMovementMapping::OnSprintPressed);
	_inputComponent->BindAction(InputConstant::ActionSprint, EInputEvent::IE_Released,
		this, &UInputToTpMovementMapping::OnSprintReleased);
	_inputComponent->BindAction(InputConstant::ActionWalk, EInputEvent::IE_Pressed,
		this, &UInputToTpMovementMapping::OnWalkPressed);
	_inputComponent->BindAction(InputConstant::ActionWalk, EInputEvent::IE_Released,
		this, &UInputToTpMovementMapping::OnWalkReleased);
}

void UInputToTpMovementMapping::Unbind() const
{
	TArray<FName> axisNames;
	axisNames.Add(InputConstant::AxisMoveHorizontal);
	axisNames.Add(InputConstant::AxisMoveVertical);
	axisNames.Add(InputConstant::AxisLookHorizontal);
	axisNames.Add(InputConstant::AxisLookVertical);

	for (auto i = _inputComponent->AxisBindings.Num() - 1; i >= 0; i--)
	{
		auto axisBinding = _inputComponent->AxisBindings[i];
		if (axisNames.Contains(axisBinding.AxisName))
		{
			_inputComponent->AxisBindings.RemoveAt(i);
		}
	}

	_inputComponent->RemoveActionBinding(InputConstant::ActionJump, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionSprint, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionSprint, EInputEvent::IE_Released);
	_inputComponent->RemoveActionBinding(InputConstant::ActionWalk, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionWalk, EInputEvent::IE_Released);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToTpMovementMapping::OnSprintPressed()
{
	_thirdPersonMovementComponent->ActivateMovementSpeedState(MovementSpeedState::Sprint);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToTpMovementMapping::OnSprintReleased()
{
	_thirdPersonMovementComponent->DeactivateMovementSpeedState(MovementSpeedState::Sprint);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToTpMovementMapping::OnWalkPressed()
{
	_thirdPersonMovementComponent->ActivateMovementSpeedState(MovementSpeedState::Walk);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToTpMovementMapping::OnWalkReleased()
{
	_thirdPersonMovementComponent->DeactivateMovementSpeedState(MovementSpeedState::Walk);
}
