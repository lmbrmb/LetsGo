#pragma once

#include "GameFramework/Pawn.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "ProtagonistPawn.generated.h"

UCLASS()
class LETSGO_API AProtagonistPawn final : public APawn
{
	GENERATED_BODY()
	
public:
	AProtagonistPawn();

	~AProtagonistPawn();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* _healthComponent;
};
