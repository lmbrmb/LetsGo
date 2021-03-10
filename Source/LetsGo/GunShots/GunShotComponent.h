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

protected:
	virtual void BeginPlay() override;
	
	///<summary>
	///[Template method] Initialization
	///</summary>
	virtual void Init() {};
	
	///<summary>
	///[Template method] Gun.OnShot event handler
	///</summary>
	virtual void OnShot(
		const int32 instigatorId,
		const USceneComponent* firePivot,
		const USceneComponent* aimProvider
	) {};
};
