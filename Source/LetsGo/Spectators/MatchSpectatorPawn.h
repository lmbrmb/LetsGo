#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "MatchSpectatorPawn.generated.h"

/// <summary>
/// Match spectator pawn
/// </summary>
UCLASS()
class LETSGO_API AMatchSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	DECLARE_EVENT(
	AMatchSpectatorPawn,
		EPossessed
		);

	EPossessed Possessed;
	
protected:
	virtual void PossessedBy(AController* NewController) override;
};
