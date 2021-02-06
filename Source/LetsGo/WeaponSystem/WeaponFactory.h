#pragma once

#include "WeaponBase.h"

class LETSGO_API WeaponFactory final
{
public:	
	WeaponFactory();

	~WeaponFactory();
	
	AWeaponBase* Create(AActor* owner, FName id);
	
private:
	TMap<const FName, const FString> _weaponAssetDictionary;
};
