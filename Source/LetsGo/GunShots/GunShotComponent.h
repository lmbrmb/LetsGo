#pragma once

#include "Components/ActorComponent.h"
#include "GunShotComponent.generated.h"

///<summary>
///Gun shot base component
///</summary>
UCLASS( ClassGroup=(Custom) )
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
	virtual void Init() {};
	
	///<summary>
	///[Template method]
	///</summary>
	virtual void OnShot(USceneComponent* firePivot, USceneComponent* aimProvider) {};
};
