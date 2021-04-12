#pragma once

#include "GameFramework/Actor.h"

#include "PickupItem.generated.h"

class APickupItem;

DECLARE_EVENT_OneParam(APickupItem, Taken, APickupItem*)

///<summary>
///Pickup item
///</summary>
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class LETSGO_API APickupItem final : public AActor
{
	GENERATED_BODY()

public:
	APickupItem();
	
	UFUNCTION(BlueprintCallable)
	FName GetId() const;

	UFUNCTION(BlueprintCallable)
	void Take();

	Taken Taken;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnTaken();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* _root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	FName _id;
};
