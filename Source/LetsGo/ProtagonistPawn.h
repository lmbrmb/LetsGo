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

	/// <summary>
	/// Known components: "Movement"
	/// </summary>
	UFUNCTION(BlueprintCallable)
	UActorComponent* GetComponent(FName componentName) const;
	
	// TODO: health component
};
