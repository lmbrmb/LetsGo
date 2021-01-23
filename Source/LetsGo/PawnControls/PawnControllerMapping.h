#pragma once

#include "CoreMinimal.h"
#include "PawnControlScheme.h"

/// <summary>
/// [abstract] [strategy] Pawn controller mapping
/// Derived class should implement Map / Unmap to define how player input controls pawn
/// </summary>
class LETSGO_API PawnControllerMapping
{
public:
	PawnControllerMapping();
	
	virtual ~PawnControllerMapping();

	virtual PawnControlScheme GetControlScheme() = 0;
	
	/// <summary>
	/// Maps player input to pawn actions
	/// </summary>
	virtual void Map(UInputComponent* playerInputComponent, APawn* pawn) = 0;

	/// <summary>
	/// Unmaps player input and pawn actions
	/// </summary>
	virtual void Unmap() = 0;
};
