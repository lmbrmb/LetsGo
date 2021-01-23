#include "PawnControllerMappingFactory.h"
#include "FirstPersonPawnControllerMapping.h"
#include "ThirdPersonCameraDirectionPawnControllerMapping.h"
#include "ThirdPersonTankPawnControllerMapping.h"

PawnControllerMappingFactory::PawnControllerMappingFactory()
{
}

PawnControllerMappingFactory::~PawnControllerMappingFactory()
{
}

PawnControllerMapping* PawnControllerMappingFactory::Create(PawnControlScheme mappingName)
{
	switch (mappingName)
	{
	case Fps:
		return new FirstPersonPawnControllerMapping();
	case ThirdPersonCameraDirection:
		return new ThirdPersonCameraDirectionPawnControllerMapping();
	case ThirdPersonTank:
		return new ThirdPersonTankPawnControllerMapping();
	default:
		return nullptr;
	}
}
