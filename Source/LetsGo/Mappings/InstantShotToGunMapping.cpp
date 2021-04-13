#include "InstantShotToGunMapping.h"

#include "LetsGo/GunShots/InstantShotComponent.h"
#include "LetsGo/WeaponSystem/GunV1.h"
#include "LetsGo/WeaponSystem/GunV2.h"
#include "LetsGo/Utils/AssertUtils.h"

void UInstantShotToGunMapping::Bind() const
{
	Super::Bind();

	auto const instantShotComponent = Cast<UInstantShotComponent>(GunShotComponent);
	AssertIsNotNull(instantShotComponent);

	auto const gunV1 = Cast<AGunV1>(GunActor);
	if (gunV1)
	{
		instantShotComponent->BulletTraced.AddUObject(gunV1, &AGunV1::OnBulletTraced);
		return;
	}

	auto const gunV2 = Cast<AGunV2>(GunActor);
	if (gunV2)
	{
		instantShotComponent->BulletTraced.AddUObject(gunV2, &AGunV2::OnBulletTraced);
		return;
	}

	DevLogger::GetLoggingChannel()->Log("Can't bind gun to instant shot. Unknown gun implementation", LogSeverity::Error);
}
