#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "FirePivotMode.h"
#include "Gun.generated.h"

const int UNDEFINED_TIME = -1.0f;
const int INITIAL_FIRE_PIVOT_INDEX = -1.0f;

///<summary>
/// Generic gun implementation
///</summary>
UCLASS()
class LETSGO_API AGun final : public AWeaponBase
{
	GENERATED_BODY()

public:
	AGun();

	DECLARE_EVENT_TwoParams(UGun, EShotPerformed, FTransform, FVector);
	
	virtual void StartFire() override;

	virtual void StopFire() override;
	
	virtual void Reload() override;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpFireStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BpShot(FTransform transform);

	EShotPerformed ShotPerformed;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpFireStopped();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpReloadStarted();
	
private:
	// Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _fireDuration = 0.5f;

	float _startFireTime = UNDEFINED_TIME;

	bool _isFireTriggered = false;
	
	// Clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _clipMax = 0;

	int _clipCurrent = 0;
	
	// Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _initialAmmoCount = 0;
	
	int _ammoCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _consumeAmmoPerShot = 1;

	// Fire pivots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _firePivots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<FirePivotMode> _firePivotMode;

	USceneComponent* GetFirePivot();

	int _firePivotIndex = INITIAL_FIRE_PIVOT_INDEX;

	// Reload
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _reloadDuration = 0;

	float _startReloadTime = UNDEFINED_TIME;

	// Private methods
	bool IsFiring() const;
	
	void ConsumeAmmo();

	bool IsClipFull() const;
	
	bool IsEnoughAmmoForShot() const;
	
	bool HasAmmoToReload() const;
	
	void StartReload();

	void FinishReload();
	
	bool IsReloading() const;

	bool IsReloadStarted() const;

	void TriggerFire(bool isFireTriggered);
};
