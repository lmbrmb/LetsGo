#pragma once

#include "PawnControllerManager.h"

class PawnControllerManagerFactory
{
public:
	explicit PawnControllerManagerFactory();
	
	~PawnControllerManagerFactory();

	PawnControllerManager* CreatePawnControllerManager(APawn* pawn) const;
};
