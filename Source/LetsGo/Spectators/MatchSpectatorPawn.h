#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "MatchSpectatorPawn.generated.h"

DECLARE_EVENT(AMatchSpectatorPawn, EPossessed);

/// <summary>
/// Match spectator pawn
/// </summary>
UCLASS()
class LETSGO_API AMatchSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	EPossessed Possessed;
	
protected:
	virtual void PossessedBy(AController* NewController) override;
};
