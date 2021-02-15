#pragma once

class LETSGO_API WeaponFactory final
{
public:
	WeaponFactory();

	~WeaponFactory();
	
	UBlueprint* GetBlueprint(FName id);
	
private:
	TMap<const FName, const FString> _weaponAssets;

	TMap<const FName, UBlueprint*> _weaponBlueprints;
};

Expose_TNameOf(WeaponFactory)
