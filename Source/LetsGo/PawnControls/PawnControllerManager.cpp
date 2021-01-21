#include "PawnControllerManager.h"
#include "FirstPersonPawnControllerMapping.h"
#include "ThirdPersonTankPawnControllerMapping.h"
#include "LetsGo/Logs/DevLogger.h"
#include "Kismet/KismetStringLibrary.h"

PawnControllerManager::PawnControllerManager(APawn* pawn)
{
	_pawn = pawn;
	auto const firstPersonPawnControllerMapping = new FirstPersonPawnControllerMapping();
	auto const thirdPersonTankPawnControllerMapping = new ThirdPersonTankPawnControllerMapping();
	_pawnControllerMappings.Insert(firstPersonPawnControllerMapping, 0);
	_pawnControllerMappings.Insert(thirdPersonTankPawnControllerMapping, 1);
}

PawnControllerManager::~PawnControllerManager()
{
	_pawnControllerMappings.Empty();
}

void PawnControllerManager::SetInputComponent(UInputComponent* inputComponent)
{
	TryUnmapCurrentMapping();
	_inputComponent = inputComponent;
}

void PawnControllerManager::CycleController()
{
	auto const mappingsLength = _pawnControllerMappings.Num();
	if(mappingsLength == 0)
	{
		DevLogger::GetLoggingChannel()->Log("No mappings available", LogSeverity::Error);
		return;
	}
	
	TryUnmapCurrentMapping();

	_mappingIndex++;

	if (_mappingIndex >= mappingsLength)
	{
		_mappingIndex = 0;
	}

	auto pawnControllerMapping = _pawnControllerMappings[_mappingIndex];

	DevLogger::GetLoggingChannel()->Log("PawnControllerMapping: " + pawnControllerMapping->GetName());
	pawnControllerMapping->Map(_inputComponent, _pawn);
}

void PawnControllerManager::TryUnmapCurrentMapping()
{
	if (_mappingIndex == MAPPING_NONE)
	{
		return;
	}
	
	auto pawnControllerMapping = _pawnControllerMappings[_mappingIndex];
	pawnControllerMapping->Unmap();
}
