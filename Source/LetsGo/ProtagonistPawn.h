#pragma once

#include "GameFramework/Pawn.h"

#include "ProtagonistPawn.generated.h"

UCLASS()
class LETSGO_API AProtagonistPawn final : public APawn
{
	GENERATED_BODY()
	
public:
	AProtagonistPawn();

	~AProtagonistPawn();

	// TODO: health component
};
