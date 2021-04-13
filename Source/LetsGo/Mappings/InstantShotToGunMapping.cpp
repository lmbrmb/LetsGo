#include "InstantShotToGunMapping.h"

#include "LetsGo/GunShots/InstantShotComponent.h"
#include "LetsGo/WeaponSystem/GunV1.h"
#include "LetsGo/WeaponSystem/GunV2.h"
#include "LetsGo/Utils/AssertUtils.h"

void UInstantShotToGunMapping::Bind() const
{
	auto const instantShotComponent = Cast<UInstantShotComponent>(GunShotComponent);

	AssertIsNotNull(instantShotComponent);
	
	Gun->ShotRequested.AddUObject(instantShotComponent, &UGunShotComponent::OnShotRequested);

	// Can't bind to interface method:
	// instantShotComponent->ShotPerformed.AddUObject(_gun, &IGun::OnShotPerformed);
	// Error: "You cannot use UObject method delegates with raw pointers"

	auto const gunV1 = Cast<AGunV1>(GunActor);
	if (gunV1)
	{
		instantShotComponent->ShotPerformed.AddUObject(gunV1, &AGunV1::OnShotPerformed);
		instantShotComponent->BulletTraced.AddUObject(gunV1, &AGunV1::OnBulletTraced);
		return;
	}

	auto const gunV2 = Cast<AGunV2>(GunActor);
	if (gunV2)
	{
		instantShotComponent->ShotPerformed.AddUObject(gunV2, &AGunV2::OnShotPerformed);
		instantShotComponent->BulletTraced.AddUObject(gunV2, &AGunV2::OnBulletTraced);
		return;
	}

	DevLogger::GetLoggingChannel()->Log("Can't bind gun to instant shot. Unknown gun implementation", LogSeverity::Error);
}
