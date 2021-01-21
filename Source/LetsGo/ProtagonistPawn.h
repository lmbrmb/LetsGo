#pragma once

#include "GameFramework/Pawn.h"
#include "PawnControls/PawnControllerManager.h"
#include "ProtagonistPawn.generated.h"

UCLASS()
class LETSGO_API AProtagonistPawn final : public APawn
{
	GENERATED_BODY()
	
public:
	AProtagonistPawn();

	~AProtagonistPawn();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transform)
	USceneComponent* TheRootComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visuals)
	USkeletalMeshComponent* SkeletalMeshComponent;

	void MoveForward(float amount);
	
	void MoveRight(float amount);

	void Move(FVector direction, float amount);
	
	void RotateRight(float amount);
	
	void Jump();
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void OnActionSwitchView();

	PawnControllerManager* _pawnControllerManager;
};
