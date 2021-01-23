#pragma once

#include "PawnControllerMapping.h"

class LETSGO_API ThirdPersonCameraDirectionPawnControllerMapping final : public PawnControllerMapping
{
public:
	ThirdPersonCameraDirectionPawnControllerMapping();
	
	~ThirdPersonCameraDirectionPawnControllerMapping();

	virtual PawnControlScheme GetControlScheme() override
	{
		return ThirdPersonCameraDirection;
	}
	
	virtual void Map(UInputComponent* playerInputComponent, APawn* pawn) override;
	
	virtual void Unmap() override;
};
