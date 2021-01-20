#pragma once

#include "GameFramework/Pawn.h"
#include "Logs/LoggingChannel.h"
#include "ProtagonistPawn.generated.h"

UCLASS()
class LETSGO_API AProtagonistPawn : public APawn
{
	GENERATED_BODY()
	
public:
	AProtagonistPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transform)
	USceneComponent* TheRootComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
	UStaticMeshComponent* StaticMeshComponent;
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	LoggingChannel* GetLoggingChannel() const;

	void OnAxisMoveForward(float amount);

	void OnAxisMoveRight(float amount);
	
	void OnActionJump();
};
