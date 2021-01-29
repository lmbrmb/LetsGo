#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class LETSGO_API APickupItem : public AActor
{
	GENERATED_BODY()

public:
	APickupItem();

	UFUNCTION(BlueprintCallable, Category = Custom)
	FString GetId();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* _root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Meta = (AllowPrivateAccess = "true"))
	FString _id;
};
