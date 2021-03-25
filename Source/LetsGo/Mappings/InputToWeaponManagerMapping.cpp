#include "InputToWeaponManagerMapping.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Utils/InputUtils.h"

void UInputToWeaponManagerMapping::Map()
{
	auto const owner = GetOwner();

	_inputComponent = owner->InputComponent;
	AssertIsNotNull(_inputComponent)

	_weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(_weaponManagerComponent)

	Bind();

	const auto healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	healthComponent->Died.AddUObject(this, &UInputToWeaponManagerMapping::OnOwnerDied);
}

void UInputToWeaponManagerMapping::ChangeWeaponDpad(const float rawAxisValue)
{
	auto const axisValue = InputUtils::GetDpadHorizontalAxis(rawAxisValue);
	if(_lastDpadHorizontalAxisValue != axisValue)
	{
		_lastDpadHorizontalAxisValue = axisValue;

		if(axisValue != 0)
		{
			_weaponManagerComponent->ChangeWeapon(axisValue);
		}
	}
}

void UInputToWeaponManagerMapping::OnOwnerDied(const UHealthComponent*, float delta) const
{
	Unbind();
}

void UInputToWeaponManagerMapping::Bind()
{
	_inputComponent->BindAxis(InputConstant::AxisChangeWeaponDpad,
		this, &UInputToWeaponManagerMapping::ChangeWeaponDpad);
	
	_inputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::StartFire);
	_inputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Released,
		_weaponManagerComponent, &UWeaponManagerComponent::StopFire);
	_inputComponent->BindAction(InputConstant::ActionReload, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::Reload);
	_inputComponent->BindAction(InputConstant::ActionPreviousWeapon, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::PreviousWeapon);
	_inputComponent->BindAction(InputConstant::ActionNextWeapon, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::NextWeapon);
	_inputComponent->BindAction(InputConstant::ActionChangeWeaponPivot, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::ChangeWeaponPivot);
}

void UInputToWeaponManagerMapping::Unbind() const
{
	TArray<FName> axisNames;
	axisNames.Add(InputConstant::AxisChangeWeaponDpad);
	
	for (auto i = _inputComponent->AxisBindings.Num() - 1; i >= 0; i--)
	{
		auto axisBinding = _inputComponent->AxisBindings[i];
		if (axisNames.Contains(axisBinding.AxisName))
		{
			_inputComponent->AxisBindings.RemoveAt(i);
		}
	}

	_inputComponent->RemoveActionBinding(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionPrimaryFire, EInputEvent::IE_Released);
	_inputComponent->RemoveActionBinding(InputConstant::ActionReload, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionPreviousWeapon, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionNextWeapon, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionChangeWeaponPivot, EInputEvent::IE_Pressed);
}
