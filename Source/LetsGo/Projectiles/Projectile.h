#pragma once

#include "GameFramework/NavMovementComponent.h"
#include "Projectile.generated.h"

class AProjectile;

DECLARE_EVENT_TwoParams(AProjectile, EHit_Projectile, AProjectile* projectile, const FHitResult& hitResult);
DECLARE_EVENT_OneParam(AProjectile, ELifeTimeExpired_Projectile, AProjectile* projectile);

/// <summary>
/// Projectile
/// </summary>
UCLASS()
class LETSGO_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	void OnCollision(const FHitResult& hitResult);

	EHit_Projectile Hit;

	ELifeTimeExpired_Projectile LifeTimeExpired;

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BpHit(const FHitResult& hitResult);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _lifeTimeSeconds = 5;

	FTimerHandle _lifeTimeTimerHandle;

	void LifeTimeExpireOnTimer();

	void SubscribeTimeManager();

	void UnsubscribeTimeManager();
};
