#pragma once

#include "PawnControllerMapping.h"

/// <summary>
/// Implements first person controller strategy
/// </summary>
class LETSGO_API FirstPersonPawnControllerMapping final : public PawnControllerMapping
{
public:
	FirstPersonPawnControllerMapping();
	
	~FirstPersonPawnControllerMapping();

	virtual void Map(UInputComponent* playerInputComponent, APawn* pawn) override;
	
	virtual void Unmap() override;

	virtual PawnControlScheme GetControlScheme() override;

private:
	UInputComponent* _playerInputComponent = nullptr;
};
