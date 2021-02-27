#pragma once

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

///<summary>
///[Abstract] Base weapon
///</summary>
UCLASS(Abstract)
class LETSGO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	
	FName GetId();

	void SetAimProvider(USceneComponent* aimProvider );
	
	virtual void StartFire() {};

	virtual void StopFire() {};
	
	virtual void Reload() {};
	
protected:
	USceneComponent* AimProvider;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName _id;
};
