#include "AvatarSfxToAvatarMapping.h"

#include "LetsGo/Sfx/AvatarSfxComponent.h"
#include "LetsGo/Utils/AssertUtils.h"

void UAvatarSfxToAvatarMapping::Map()
{
	auto const actor = GetOwner();

	_avatarSfxComponent = actor->FindComponentByClass<UAvatarSfxComponent>();
	AssertIsNotNull(_avatarSfxComponent);

	auto const avatar = Cast<AAvatar>(actor);
	AssertIsNotNull(avatar);

	auto const isInitialized = avatar->IsInitialized();
	if (isInitialized)
	{
		OnAvatarInitialized(avatar);
	}
	else
	{
		avatar->Initialized.AddUObject(this, &UAvatarSfxToAvatarMapping::OnAvatarInitialized);
	}
}

void UAvatarSfxToAvatarMapping::OnAvatarInitialized(const AAvatar* avatar)
{
	auto const skinId = avatar->GetSkinId();
	_avatarSfxComponent->SetSkinId(skinId);

	StartDestroyTask();
}
