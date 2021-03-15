#include "AvatarToWeaponManagerMapping.h"

void UAvatarToWeaponManagerMapping::Map()
{
	auto const actor = GetOwner();
	auto const avatar = Cast<AAvatar>(actor);
	AssertIsNotNull(avatar);
	
	const auto weaponManagerComponent = actor->FindComponentByClass<UWeaponManagerComponent>();
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
