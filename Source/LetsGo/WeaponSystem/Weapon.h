#pragma once

#include "UObject/Interface.h"
#include "Weapon.generated.h"

/// <summary>
/// [Generated] Must-have class for IWeapon
/// </summary>
UINTERFACE(MinimalAPI)
class UWeapon : public UInterface
{
	GENERATED_BODY()
};

///<summary>
/// Weapon interface
///</summary>
class LETSGO_API IWeapon
{
	GENERATED_BODY()

	// Not virtual - default implementation is ok
public:
	DECLARE_EVENT_OneParam(
		IGun,
		EWeaponInitialized,
		IWeapon*
		);

	EWeaponInitialized WeaponInitialized;
	
	bool IsWeaponInitialized() const;

	void InitializeWeapon(const FName& id, const int32 playerId);

	FName GetId() const;

	int32 GetPlayerId() const;
	
private:
	FName _id;

	int32 _playerId;
	
	bool _isWeaponInitialized = false;
};
