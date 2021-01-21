#pragma once

#include "PawnControllerMapping.h"
#include "LetsGo/ProtagonistPawn.h"

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

	virtual FString GetName() override;

private:
	AProtagonistPawn* _protagonistPawn = nullptr;

	UInputComponent* _playerInputComponent = nullptr;
};
