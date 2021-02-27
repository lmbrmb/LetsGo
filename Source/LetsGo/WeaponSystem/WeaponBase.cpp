#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
}

FName AWeaponBase::GetId()
{
	return _id;
}

void AWeaponBase::SetAimProvider(USceneComponent* aimProvider)
{
	AimProvider = aimProvider;
}
