#pragma once

class LETSGO_API WeaponFactory final
{
public:	
	WeaponFactory();

	~WeaponFactory();
	
	UBlueprint* GetBlueprint(FName id);
	
private:
	TMap<const FName, const FString> _weaponAssetDictionary;

	TMap<const FName, UBlueprint*> _weaponBlueprintsDictionary;
	
	bool _isInitialized = false;
};
