#pragma once

///<summary>
///Pickup item factory
///</summary>
class LETSGO_API PickupItemFactory final
{
public:
	PickupItemFactory();
	
	~PickupItemFactory();

	UBlueprint* GetBlueprint(FName id);

private:
	TMap<const FName, const FString> _pickupAssets;

	TMap<const FName, UBlueprint*> _pickupBlueprints;
};

Expose_TNameOf(PickupItemFactory)
