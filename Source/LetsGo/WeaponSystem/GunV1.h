#pragma once

#include "WeaponBase.h"
#include "FirePivotMode.h"
#include "Gun.h"
#include "GunState.h"

#include "GunV1.generated.h"

///<summary>
/// Generic gun implementation v1.
/// Need enough bullets in clip in order to fire.
/// Reloading fills clip, bullets are taken from ammo provider.
///</summary>
UCLASS()
class LETSGO_API AGunV1 final : public AWeaponBase, public IGun
{
	GENERATED_BODY()

public:
	AGunV1();

	// IGun.Init implementation
	virtual void Init(const int32 instigatorId, AmmoProvider* ammoProvider, USceneComponent* aimProvider) override;

	// IGun.StartFire implementation
	virtual void StartFire() override;

	// IGun.StopFire implementation
	virtual void StopFire() override;

	// IGun.Reload implementation
	virtual void Reload() override;
	
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
	int32 _instigatorId;
	
	const int UNDEFINED_TIME = -1.0f;
	
	const int INITIAL_FIRE_PIVOT_INDEX = -1.0f;

	// State
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _consumeAmmoPerShot = 1;

	AmmoProvider* _ammoProvider;
	
	// Fire pivots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _firePivots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FFirePivotMode _firePivotMode;

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

	// Aim
	USceneComponent* _aimProvider;
	
	// Private methods
	bool IsShooting() const;
	
	void StartShot();

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

	void TriggerFire(const bool isFireTriggered);
};
