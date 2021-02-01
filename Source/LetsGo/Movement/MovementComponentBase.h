#pragma once

#include "MovementComponentBase.generated.h"

///<summary>
/// [Abstract] Movement component
///</summary>
UCLASS( Abstract, ClassGroup=(_Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UMovementComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UMovementComponentBase();

	virtual ~UMovementComponentBase();

	virtual void BeginPlay() override final;
	
	virtual void Init(AActor* actor) {};
	
	virtual void MapPlayerInput(UInputComponent* playerInputComponent) {};
};
