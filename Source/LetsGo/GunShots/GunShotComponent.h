#pragma once

#include "Components/ActorComponent.h"
#include "GunShotComponent.generated.h"

///<summary>
///Gun shot base component
///</summary>
UCLASS( Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UGunShotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGunShotComponent();

	virtual void BeginPlay() override;
protected:
	///<summary>
	///[Template method]
	///</summary>
	virtual void Init();
	
	///<summary>
	///[Template method]
	///</summary>
	virtual void OnShot(FTransform pivotTransform, FVector shotDirection);
};
