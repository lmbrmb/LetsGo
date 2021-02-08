#pragma once

#include "WeaponBase.h"
#include "FirePivotMode.h"
#include "Gun.generated.h"

const int UNDEFINED_RELOADING_TIME = -1;
const int INITIAL_FIRE_PIVOT_INDEX = -1;

///<summary>
/// Generic gun implementation
///</summary>
UCLASS()
class LETSGO_API AGun final : public AWeaponBase
{
	GENERATED_BODY()

public:
	AGun();
	
	virtual void Fire() override;

	virtual void Reload() override;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnFire(FTransform transform);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReloadingStart();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _clipMax = 0;

	int _clipCurrent = 0;

	int _ammoCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _consumeAmmoPerShot = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _firePivots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<FirePivotMode> _firePivotMode;

	USceneComponent* GetPivot();

	int _firePivotIndex = INITIAL_FIRE_PIVOT_INDEX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _initialAmmoCount = 10;
	
	void ConsumeAmmo();

	bool IsEnoughAmmoForShot();
	
	bool CanReload() const;
	
	void StartReloading();

	void FinishReloading();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _reloadTime = 0;
	
	float _startReloadTime = UNDEFINED_RELOADING_TIME;
	
	bool IsReloading() const;

	bool IsReloadingRequested() const;
};
