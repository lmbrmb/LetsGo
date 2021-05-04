#pragma once

#include "IGun.h"
#include "GunState.h"

#include "GunV2.generated.h"

///<summary>
/// Generic gun implementation, v2.
/// No clip / reload simulation.
/// On fire ammo taken straight from ammo provider.
///</summary>
UCLASS()
class LETSGO_API AGunV2 final : public AActor, public IGun
{
	GENERATED_BODY()

public:
	AGunV2();

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
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

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

	UFUNCTION(BlueprintCallable)
	FVector GetTargetAimLocation() const;

private:
	const int UNDEFINED_TIME = -1.0f;
	
	const int INITIAL_FIRE_PIVOT_INDEX = -1.0f;
	
	GunState _state = GunState::None;
	
	void SetState(const GunState state);
	
	void ProcessIdleState();

	void ProcessShootingState();

	TMap<GunState, TFunction<void()>> _stateProcessors;

	TFunction<void()> _stateProcessor = nullptr;
	
	// Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _shotDuration = 0.5f;

	float _shotStartTime = UNDEFINED_TIME;

	bool _isFireTriggered = false;

	// Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _consumeAmmoPerShot = 1;
	
	// Fire pivot
	USceneComponent* _firePivot;

	// Private methods
	bool IsShooting() const;

	void StartShot();

	int GetAmmoCount() const;

	void ConsumeAmmo(const int amount) const;

	void TriggerFire(bool isFireTriggered);
};
