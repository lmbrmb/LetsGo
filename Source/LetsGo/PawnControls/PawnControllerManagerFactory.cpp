#include "PawnControllerManagerFactory.h"

PawnControllerManagerFactory::PawnControllerManagerFactory()
{
}

PawnControllerManagerFactory::~PawnControllerManagerFactory()
{
}

PawnControllerManager* PawnControllerManagerFactory::CreatePawnControllerManager(APawn* pawn) const
{
	check(pawn != nullptr);
	return new PawnControllerManager(pawn);
}
