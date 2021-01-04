// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtagonistPawn.h"
#include "ProtagonistPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class LETSGO_API AProtagonistPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProtagonistPawn* Pawn;
};
