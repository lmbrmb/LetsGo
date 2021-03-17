#pragma once

#include "FirePivotMode.h"
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

	// IGun implementation
	virtual void StartFire() override;

	// IGun implementation
	virtual void StopFire() override;

	// IGun implementation
	virtual void Reload() override;

	// IGun implementation
	virtual void OnShotPerformed(const bool isAnyBulletDamaged) override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnFireStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnFireStopped();
	
private:
	const int UNDEFINED_TIME = -1.0f;
	
	const int INITIAL_FIRE_PIVOT_INDEX = -1.0f;
	
	GunState _state = GunState::Idle;

	void SetState(const GunState state);

	void ProcessIdleState();

	void ProcessShootingState();

	// Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _shotDuration = 0.5f;

	float _shotStartTime = UNDEFINED_TIME;

	bool _isFireTriggered = false;

	// Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int _consumeAmmoPerShot = 1;
	
	// Fire pivots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _firePivots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FFirePivotMode _firePivotMode;

	USceneComponent* GetFirePivot();

	int _firePivotIndex = INITIAL_FIRE_PIVOT_INDEX;
	
	// Private methods
	bool IsShooting() const;

	void StartShot();

	bool IsEnoughAmmoForShot() const;

	int GetAmmoCount() const;

	void ConsumeAmmo(const int amount) const;

	void TriggerFire(bool isFireTriggered);
};
