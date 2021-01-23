#pragma once
#include "PawnControllerMapping.h"
#include "PawnControlScheme.h"

/// <summary>
/// Concrete factory which can create many pawn control mappings
/// </summary>
class PawnControllerMappingFactory final
{
public:
	PawnControllerMappingFactory();

	~PawnControllerMappingFactory();

	/// <summary>
	/// Creates requested mapping if known
	/// </summary>
	/// <returns>Requested mapping if known, otherwise nullptr </returns>
	PawnControllerMapping* Create(PawnControlScheme mappingName);
};
