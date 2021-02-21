#pragma once
#include "UObject/NameTypes.h"

class LETSGO_API InputConstant final
{
public:
	static const FName ActionAnyKey;
	
	static const FName AxisMoveHorizontal;

	static const FName AxisMoveVertical;

	static const FName AxisLookHorizontal;

	static const FName AxisLookVertical;

	static const FName ActionJump;

	static const FName ActionSprint;

	static const FName ActionPrimaryFire;

	static const FName ActionReload;

	static const FName ActionNextWeapon;

	static const FName ActionPreviousWeapon;

	static const FName AxisChangeWeaponDpad;

	static const FName ActionChangeWeaponPivot;
};
