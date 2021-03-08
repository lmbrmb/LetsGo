#include "AvatarToWeaponManagerMapping.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/Logs/DevLogger.h"

void UAvatarToWeaponManagerMapping::Map()
{
	auto const actor = GetOwner();

	auto const avatar = Cast<AAvatar>(actor);
	if (!avatar)
	{
		DevLogger::GetLoggingChannel()->LogValue(
			"Component is not found: ",
			AAvatar::StaticClass()->GetName(),
			LogSeverity::Error
		);
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

	auto const avatarData = avatar->GetAvatarData();
	if(avatarData)
	{
		OnAvatarDataSet(avatarData);
	}
	avatar->AvatarDataSet.AddUObject(this, &UAvatarToWeaponManagerMapping::OnAvatarDataSet);
}

bool UAvatarToWeaponManagerMapping::ShouldDestroyAfterMapping() const
{
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UAvatarToWeaponManagerMapping::OnAvatarDataSet(const AvatarData* avatarData)
{
	_weaponManagerComponent->SetInstigatorId(avatarData->GetPlayerId());
}
