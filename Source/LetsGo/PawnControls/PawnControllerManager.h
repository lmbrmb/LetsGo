#pragma once

#include "PawnControllerMapping.h"

typedef int MappingIndex;

const MappingIndex MAPPING_NONE = -1;

/// <summary>
/// Pawn controller manager. Controls input / view strategies
/// </summary>
class LETSGO_API PawnControllerManager final
{
public:
	explicit PawnControllerManager(APawn* pawn);
	
	~PawnControllerManager();
	
	void SetInputComponent(UInputComponent* inputComponent);

	void CycleController();
	
private:
	APawn* _pawn = nullptr;

	UInputComponent* _inputComponent = nullptr;

	TArray<PawnControllerMapping*> _pawnControllerMappings;

	MappingIndex _mappingIndex = MAPPING_NONE;

	void TryUnmapCurrentMapping();
};
