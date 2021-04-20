#pragma once

#include "IGun.h"
#include "GunState.h"

#include "GunV1.generated.h"

///<summary>
/// Generic gun implementation v1.
/// Need enough bullets in clip in order to fire.
/// Reloading fills clip, bullets are taken from ammo provider.
///</summary>
UCLASS()
class LETSGO_API AGunV1 final : public AActor, public IGun
{
	GENERATED_BODY()

public:
	AGunV1();

	// IWeapon implementation
	virtual bool IsRequestReady() const override;

	// IGun implementation
	virtual void StartFire() override;

	// IGun implementation
	virtual void StopFire() override;

	// IGun implementation
	virtual void Reload() override;

	// IGun implementation
	virtual bool IsEnoughAmmoForShot() const override;

	// IGun implementation
	virtual void OnShotPerformed(const bool isAnyBulletDamaged) override;

	// IGun implementation
	virtual void OnBulletTraced(const bool isDamaged, const FHitResult& hitResult) override;

protected:
	UFUNCTION(BlueprintCallable)
	void SetFirePivot(USceneComponent* firePivot);

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnFireStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnFireStopped();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnShotRequest(const USceneComponent* firePivot);

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnBulletTraced(const bool isDamaged, const FHitResult& hitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnReloadStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnReloadFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAmmoLoaded();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	const int UNDEFINED_TIME = -1.0f;
	
	const int INITIAL_FIRE_PIVOT_INDEX = -1.0f;

	// State
	GunState _state = GunState::None;

	void SetState(GunState state);
	
	void ProcessIdleState();

	void ProcessReloadingState();

	void ProcessShootingState();

	TMap<GunState, TFunction<void()>> _stateProcessors;

	TFunction<void()> _stateProcessor = nullptr;
	
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
	
	// Fire pivot
	USceneComponent* _firePivot;

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

	bool IsClipFull() const;
	
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
