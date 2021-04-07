#include "InputToFpMovementMapping.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Utils/AssertUtils.h"

void UInputToFpMovementMapping::Map()
{
	auto const owner = GetOwner();

	_inputComponent = owner->InputComponent;
	AssertIsNotNull(_inputComponent);

	_firstPersonMovementComponent = owner->FindComponentByClass<UFirstPersonMovementComponent>();
	AssertIsNotNull(_firstPersonMovementComponent);

	Bind();

	const auto healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);
	
	healthComponent->Died.AddUObject(this, &UInputToFpMovementMapping::OnOwnerDied);
}

void UInputToFpMovementMapping::OnOwnerDied(const UHealthComponent*, float delta) const
{
	Unbind();
}

void UInputToFpMovementMapping::Bind() const
{
	_inputComponent->BindAxis(InputConstant::AxisMoveHorizontal, _firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddActorRightMovementInput);
	_inputComponent->BindAxis(InputConstant::AxisMoveVertical, _firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddActorForwardMovementInput);
	_inputComponent->BindAxis(InputConstant::AxisLookHorizontal, _firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddActorYawInput);
	_inputComponent->BindAxis(InputConstant::AxisLookVertical, _firstPersonMovementComponent,
		&UFirstPersonMovementComponent::AddCameraPitchInput);
	
	_inputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed,
		_firstPersonMovementComponent, &UFirstPersonMovementComponent::PerformJump);
	_inputComponent->BindAction(InputConstant::ActionSprint, EInputEvent::IE_Pressed,
		_firstPersonMovementComponent, &UFirstPersonMovementComponent::StartSprint);
	_inputComponent->BindAction(InputConstant::ActionSprint, EInputEvent::IE_Released,
		_firstPersonMovementComponent, &UFirstPersonMovementComponent::StopSprint);
}

void UInputToFpMovementMapping::Unbind() const
{
	TArray<FName> axisNames;
	axisNames.Add(InputConstant::AxisMoveHorizontal);
	axisNames.Add(InputConstant::AxisMoveVertical);
	axisNames.Add(InputConstant::AxisLookHorizontal);
	axisNames.Add(InputConstant::AxisLookVertical);
	
	for (auto i = _inputComponent->AxisBindings.Num() - 1; i >=0 ; i--)
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
}
