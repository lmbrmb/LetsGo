#pragma once

#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "PawnControls/PawnControllerMapping.h"
#include "PawnControls/PawnControlScheme.h"
#include "ProtagonistPawn.generated.h"

UCLASS()
class LETSGO_API AProtagonistPawn final : public APawn
{
	GENERATED_BODY()
	
public:
	AProtagonistPawn();

	~AProtagonistPawn();
	
	// TODO: health component
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	/// <summary>
	/// 1. Root transform (position, rotation)
	/// 2. Pawn's ground collider. Physics handling (gravity, collisions)
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Root, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* RootCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Control, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<PawnControlScheme> ControlScheme;
	
	PawnControllerMapping* _pawnControllerMapping;
};
