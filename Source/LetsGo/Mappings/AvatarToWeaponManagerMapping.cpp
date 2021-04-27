#include "AvatarToWeaponManagerMapping.h"

#include "LetsGo/Utils/AssertUtils.h"

void UAvatarToWeaponManagerMapping::Map()
{
	auto const owner = GetOwner();
	auto const avatar = Cast<AAvatar>(owner);
	AssertIsNotNull(avatar);
	
	auto const weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManagerComponent);

	_weaponManagerComponent = weaponManagerComponent;

	auto const isInitialized = avatar->IsInitialized();
	if(isInitialized)
	{
		OnAvatarInitialized(avatar);
	}
	else
	{
		avatar->Initialized.AddUObject(this, &UAvatarToWeaponManagerMapping::OnAvatarInitialized);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UAvatarToWeaponManagerMapping::OnAvatarInitialized(const AAvatar* avatar)
{
	_weaponManagerComponent->SetPlayerId(avatar->GetPlayerId());
	StartDestroyTask();
}
