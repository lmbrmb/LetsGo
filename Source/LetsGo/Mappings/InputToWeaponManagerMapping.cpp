#include "InputToWeaponManagerMapping.h"

#include "LetsGo/Input/InputConstant.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/InputUtils.h"

void UInputToWeaponManagerMapping::Map()
{
	auto const owner = GetOwner();

	_inputComponent = owner->InputComponent;
	AssertIsNotNull(_inputComponent)

	_weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(_weaponManagerComponent)

	Bind();

	auto const healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(healthComponent);

	healthComponent->Died.AddUObject(this, &UInputToWeaponManagerMapping::OnOwnerDied);
}

void UInputToWeaponManagerMapping::OnChangeWeaponDpad(const float rawAxisValue)
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToWeaponManagerMapping::OnEquipMachinegun()
{
	_weaponManagerComponent->ChangeWeapon(_machinegunId);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToWeaponManagerMapping::OnEquipShotgun()
{
	_weaponManagerComponent->ChangeWeapon(_shotgunId);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToWeaponManagerMapping::OnEquipRocketLauncher()
{
	_weaponManagerComponent->ChangeWeapon(_rocketLauncherId);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToWeaponManagerMapping::OnEquipRailgun()
{
	_weaponManagerComponent->ChangeWeapon(_railgunId);
}

void UInputToWeaponManagerMapping::OnOwnerDied(UHealthComponent* healthComponent, float delta) const
{
	Unbind();
}

void UInputToWeaponManagerMapping::Bind()
{
	_inputComponent->BindAxis(InputConstant::AxisChangeWeaponDpad,
		this, &UInputToWeaponManagerMapping::OnChangeWeaponDpad);
	
	_inputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::StartFire);
	_inputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Released,
		_weaponManagerComponent, &UWeaponManagerComponent::StopFire);
	_inputComponent->BindAction(InputConstant::ActionReload, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::Reload);
	_inputComponent->BindAction(InputConstant::ActionEquipPreviousWeapon, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::PreviousWeapon);
	_inputComponent->BindAction(InputConstant::ActionEquipNextWeapon, EInputEvent::IE_Pressed,
		_weaponManagerComponent, &UWeaponManagerComponent::NextWeapon);
	_inputComponent->BindAction(InputConstant::ActionEquipMachinegun, EInputEvent::IE_Pressed,
		this, &UInputToWeaponManagerMapping::OnEquipMachinegun);
	_inputComponent->BindAction(InputConstant::ActionEquipShotgun, EInputEvent::IE_Pressed,
		this, &UInputToWeaponManagerMapping::OnEquipShotgun);
	_inputComponent->BindAction(InputConstant::ActionEquipRocketLauncher, EInputEvent::IE_Pressed,
		this, &UInputToWeaponManagerMapping::OnEquipRocketLauncher);
	_inputComponent->BindAction(InputConstant::ActionEquipRailgun, EInputEvent::IE_Pressed,
		this, &UInputToWeaponManagerMapping::OnEquipRailgun);
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
	_inputComponent->RemoveActionBinding(InputConstant::ActionEquipPreviousWeapon, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionEquipNextWeapon, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionEquipMachinegun, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionEquipShotgun, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionEquipRocketLauncher, EInputEvent::IE_Pressed);
	_inputComponent->RemoveActionBinding(InputConstant::ActionEquipRailgun, EInputEvent::IE_Pressed);
}
