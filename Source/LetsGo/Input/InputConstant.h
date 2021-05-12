#pragma once
#include "UObject/NameTypes.h"

class LETSGO_API InputConstant final
{
public:
	// Action

	static const FName ActionAnyKey;

	static const FName ActionJump;

	static const FName ActionSprint;

	static const FName ActionWalk;

	static const FName ActionPrimaryFire;

	static const FName ActionReload;

	static const FName ActionEquipNextWeapon;

	static const FName ActionEquipPreviousWeapon;

	static const FName ActionEquipMachinegun;

	static const FName ActionEquipShotgun;

	static const FName ActionEquipRocketLauncher;

	static const FName ActionEquipRailgun;

	static const FName ActionEscape;

	// Axis

	static const FName AxisMoveHorizontal;

	static const FName AxisMoveVertical;

	static const FName AxisLookHorizontal;

	static const FName AxisLookVertical;

	static const FName AxisChangeWeaponDpad;
};
