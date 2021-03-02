#pragma once

#include "WeaponBase.h"
#include "FirePivotMode.h"
#include "Gun.h"
#include "GunState.h"
#include "GunV2.generated.h"

///<summary>
/// Generic gun implementation, v2.
/// No clip / reload simulation.
/// On fire ammo taken straight from ammo provider.
///</summary>
UCLASS()
class LETSGO_API AGunV2 final : public AWeaponBase, public IGun
{
	GENERATED_BODY()

public:
	AGunV2();

	// IGun.Init implementation
	virtual void Init(AmmoProvider* ammoProvider, USceneComponent* aimProvider) override;

	// IGun.StartFire implementation
	virtual void StartFire() override;

	// IGun.StopFire implementation
	virtual void StopFire() override;

	// IGun.Reload implementation
	virtual void Reload() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpFireStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BpFireStopped();

	UFUNCTION(BlueprintImplementableEvent)
	void BpAmmoLoaded();

protected:
	virtual void Tick(float DeltaSeconds) override;

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

	AmmoProvider* _ammoProvider;
	
	// Fire pivots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _firePivots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FirePivotMode _firePivotMode;

	USceneComponent* GetFirePivot();

	int _firePivotIndex = INITIAL_FIRE_PIVOT_INDEX;

	// Aim
	USceneComponent* _aimProvider;
	
	// Private methods
	bool IsShooting() const;

	void StartShot();

	bool IsEnoughAmmoForShot() const;

	int GetAmmoCount() const;

	void ConsumeAmmo(const int amount) const;

	void TriggerFire(bool isFireTriggered);
};
