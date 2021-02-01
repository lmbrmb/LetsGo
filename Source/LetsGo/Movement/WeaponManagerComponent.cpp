#include "WeaponManagerComponent.h"
#include "LetsGo/InputConstant.h"
#include "LetsGo/Logs/DevLogger.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	auto const actor = GetOwner();
	
	auto const inputComponent = actor->InputComponent;
	if(inputComponent)
	{
		MapPlayerInput(inputComponent);
	}
}

void UWeaponManagerComponent::MapPlayerInput(UInputComponent* playerInputComponent)
{
	playerInputComponent->BindAction(InputConstant::ActionPrimaryFire, EInputEvent::IE_Pressed, this, &UWeaponManagerComponent::PrimaryFire);
}

void UWeaponManagerComponent::PrimaryFire()
{
	DevLogger::GetLoggingChannel()->Log("PrimaryFire");
}

void UWeaponManagerComponent::OnInventoryItemAdded(FName itemId)
{
	DevLogger::GetLoggingChannel()->Log("WeaponManager. OnInventoryItemAdded", LogSeverity::Warning);
}

void UWeaponManagerComponent::OnInventoryItemRemoved(FName itemId)
{
	DevLogger::GetLoggingChannel()->Log("WeaponManager. OnInventoryItemRemoved", LogSeverity::Warning);
}
