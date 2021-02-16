#pragma once

#include "GunShotComponent.h"
#include "RailgunShotComponent.generated.h"

///<summary>
///Railgun shot
///</summary>
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
//EditInlineNew
class LETSGO_API URailgunShotComponent final : public UGunShotComponent
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	
	virtual void OnShot(FTransform pivotTransform, FVector shotDirection) override;
private:
	ULineBatchComponent* _lineBatcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _rayDistance = 100;
};
