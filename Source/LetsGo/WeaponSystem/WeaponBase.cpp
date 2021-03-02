#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
}

FName AWeaponBase::GetId()
{
	return _id;
}

void AWeaponBase::SetId(const FName id)
{
	_id = id;
}
