#pragma once

///<summary>
/// Blueprint factory.
/// Allows to get blueprint by asset name
/// Derived factory should expose it's name for proper dependency injection
///</summary>
class BlueprintFactory
{
public:
	BlueprintFactory();

	virtual ~BlueprintFactory();
	
	UBlueprint* GetBlueprint(const FName id);

protected:
	/// <summary>
	/// Known assets. Should be initialized in ctor
	/// </summary>
	TMap<const FName, const FString> Assets;

	/// <summary>
	/// Loaded blueprints cache
	/// </summary>
	TMap<const FName, UBlueprint*> Blueprints;
};
