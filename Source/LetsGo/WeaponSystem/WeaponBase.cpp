#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
}

void AWeaponBase::SetAimProvider(USceneComponent* aimProvider)
{
	AimProvider = aimProvider;
}
