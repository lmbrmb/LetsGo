#pragma once

#include "CoreMinimal.h"

#include "AmmoProvider.h"
#include "WeaponBase.h"
#include "FirePivotMode.h"
#include "GunState.h"
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

	DECLARE_EVENT_TwoParams(UGun, EShotPerformed, USceneComponent*, USceneComponent*);
	
	virtual void StartFire() override;

	virtual void StopFire() override;
	
	virtual void Reload() override;

	void SetAmmoProvider(AmmoProvider* ammoProvider);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpFireStarted();

	EShotPerformed ShotPerformed;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpFireStopped();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpReloadStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BpReloadFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void BpAmmoLoaded();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
private:
	GunState _state = GunState::Idle;

	void SetState(GunState state);
	
	void ProcessIdleState();

	void ProcessReloadingState();

	void ProcessShootingState();
	
	// Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _shotDuration = 0.5f;
	
	float _shotStartTime = UNDEFINED_TIME;

	bool _isFireTriggered = false;
	
	// Clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _clipMax = 0;

	int _clipCurrent = 0;
	
	// Ammo
	AmmoProvider* _ammoProvider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _consumeAmmoPerShot = 1;

	// Fire pivots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _firePivots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FirePivotMode _firePivotMode;

	USceneComponent* GetFirePivot();

	int _firePivotIndex = INITIAL_FIRE_PIVOT_INDEX;

	// Reload

	/// <summary>
	/// How much time it takes to start loading ammo after reloading triggered
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _loadAmmoDelay = 0.5f;
	
	float _reloadStartTime = UNDEFINED_TIME;
	
	/// <summary>
	/// How much time it takes to insert one portion of ammo
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _ammoLoadDuration = 0;

	/// <summary>
	/// How much ammo will be added to clip per one load time
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _ammoPerLoad = 1;
	
	float _ammoLoadStartTime = UNDEFINED_TIME;
	
	// Private methods
	bool IsShooting() const;
	
	void StartShot();

	void AddToClip(const int amount);
	
	void ConsumeClip(const int amount);

	bool IsClipFull() const;
	
	bool IsEnoughAmmoForShot() const;
	
	bool HasAmmoToLoad() const;
	
	void StartReload();
	
	int GetAmmoCount() const;

	void ConsumeAmmo(const int amount) const;
	
	void LoadAmmo();
	
	void FinishReload();

	bool IsLoadAmmoDelayActive() const;
	
	bool IsLoadingAmmo() const;

	bool IsLoadingAmmoStarted() const;

	void TriggerFire(bool isFireTriggered);
};
