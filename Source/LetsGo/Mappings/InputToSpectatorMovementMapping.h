#pragma once

#include "MappingComponent.h"
#include "LetsGo/Spectators/MatchSpectatorPawn.h"


#include "InputToSpectatorMovementMapping.generated.h"

/// <summary>
/// Input to spectator pawn movement mapping
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UInputToSpectatorMovementMapping : public UMappingComponent
{
	GENERATED_BODY()

protected:
	virtual void Map() override;

private:
	void OnPossessed();

	AMatchSpectatorPawn* _matchSpectatorPawn;
};
