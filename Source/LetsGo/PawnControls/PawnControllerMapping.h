#pragma once

#include "CoreMinimal.h"

/// <summary>
/// [abstract] Pawn controller mapping
/// Derived class should implement strategy for Map / Unmap to define player view and how player input controls pawn actions
/// Example: First person controller / Third person controller
/// </summary>
class LETSGO_API PawnControllerMapping
{
public:
	PawnControllerMapping();
	
	virtual ~PawnControllerMapping();

	virtual FString GetName() = 0;
	
	/// <summary>
	/// Maps player input to pawn actions
	/// </summary>
	virtual void Map(UInputComponent* playerInputComponent, APawn* pawn) = 0;

	/// <summary>
	/// Unmaps player input and pawn actions
	/// </summary>
	virtual void Unmap() = 0;
};
