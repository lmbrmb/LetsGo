#include "InputToWeaponManagerMapping.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Utils/InputUtils.h"

void UInputToWeaponManagerMapping::Map()
{
	auto const actor = GetOwner();

	auto const inputComponent = actor->InputComponent;
	if (!inputComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No input component", LogSeverity::Error);
		return;
	}

	const auto weaponManagerComponent = actor->FindComponentByClass<UWeaponManagerComponent>();
	if (!weaponManagerComponent)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ",
			UWeaponManagerComponent::StaticClass()->GetName(),
			LogSeverity::Error
		);
		return;
	}

	_weaponManagerComponent = weaponManagerComponent;
	
	inputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed,
		weaponManagerComponent, &UWeaponManagerComponent::StartFire);
	inputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Released,
		weaponManagerComponent, &UWeaponManagerComponent::StopFire);
	inputComponent->BindAction(InputConstant::ActionReload, EInputEvent::IE_Pressed,
		weaponManagerComponent, &UWeaponManagerComponent::Reload);
	inputComponent->BindAction(InputConstant::ActionPreviousWeapon, EInputEvent::IE_Pressed,
		weaponManagerComponent, &UWeaponManagerComponent::PreviousWeapon);
	inputComponent->BindAction(InputConstant::ActionNextWeapon, EInputEvent::IE_Pressed,
		weaponManagerComponent, &UWeaponManagerComponent::NextWeapon);
	inputComponent->BindAxis(InputConstant::AxisChangeWeaponDpad,
		this, &UInputToWeaponManagerMapping::ChangeWeaponDpad);
	inputComponent->BindAction(InputConstant::ActionChangeWeaponPivot, EInputEvent::IE_Pressed,
		weaponManagerComponent, &UWeaponManagerComponent::ChangeWeaponPivot);
}

bool UInputToWeaponManagerMapping::ShouldDestroyAfterMapping() const
{
	return false;
}

void UInputToWeaponManagerMapping::ChangeWeaponDpad(float rawAxisValue)
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