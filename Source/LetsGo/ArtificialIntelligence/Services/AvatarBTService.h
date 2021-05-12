#pragma once

#include "BehaviorTree/BTService.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/HealthSystem/HealthComponent.h"
#include "LetsGo/Pickups/PickupItem.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "AvatarBTService.generated.h"

/// <summary>
/// Avatar behavior tree service.
/// Updates: health, weapon, enemy
/// </summary>
UCLASS()
class LETSGO_API UAvatarBTService : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TEnumAsByte<ECollisionChannel> _traceCollisionChannel = ECC_Pawn;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _enemyActorKeyName = "EnemyActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _enemyDetectionTimeKeyName = "EnemyDetectionTime";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _pickupActorKeyName = "PickupActor";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isEnemyInLineOfSightKeyName = "IsEnemyInLineOfSight";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Trace)
	float _detectionRadius = 2000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Trace)
	float _distanceSquaredBaseFactor = 50000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Trace)
	FVector _avatarRayCastLocationOffset = FVector::UpVector * 75;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Weapon)
	TArray<FName> _lowTierWeapons;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Weapon)
	float _minAmmoPercent = 0.15f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = PickUpItems)
	TArray<FName> _normalHealthPickups;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = PickUpItems)
	TArray<FName> _superHealthPickups;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = PickUpItems)
	TArray<FName> _weaponPickups;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = PickUpItems)
	float _lowHealthValue = 60;

	FCollisionShape _collisionShape;

	bool _isCollisionShapeInitialized = false;

	FHitResult _hitResult;

	TArray<FHitResult> _hitResults;

	TArray<AAvatar*> _enemies;

	TArray<APickupItem*> _pickupItems;

	void Update(UBlackboardComponent* blackboardComponent);

	bool IsNeedHealth(UHealthComponent* healthComponent) const;

	bool IsNeedWeapon(UWeaponManagerComponent* weaponManagerComponent) const;

	bool IsLowTierWeapon(IWeapon* weapon) const;

	bool IsEnoughAmmo(IGun* gun) const;

	void Detect(
		AAvatar* selfAvatar,
		const FVector& selfLocation
	);
	
	bool TryProcessEnemy(
		AActor* selfActor,
		AActor* otherActor,
		const TeamId& selfTeamId
	);

	void TryAddEnemy(
		AActor* selfActor,
		AAvatar* enemyAvatar,
		const TeamId& selfTeamId
	);
	
	bool TryProcessPickup(
		AActor* otherActor,
		const FVector& selfLocation
	);

	bool CanTakePickup(
		APickupItem* pickupItem,
		const bool isNeedHealth,
		const bool isNeedWeapon
	) const;

	void TryAddPickup(
		APickupItem* pickupItem
	);

	bool IsKnownPickup(APickupItem* pickupItem) const;

	static bool IsPickupValid(
		APickupItem* pickupItem
	);
	
	void UpdateEnemy(
		UBlackboardComponent* blackboardComponent,
		AActor* selfActor,
		const FVector& selfLocation
	);

	float GetEnemyFactor(
		AActor* enemyActor,
		const FVector& selfLocation,
		const FVector& selfForward
	) const;

	void UpdatePickup(
		AActor* selfActor,
		UBlackboardComponent* blackboardComponent,
		const FVector& selfLocation
	) const;
	
	AAvatar* GetCurrentEnemy(
		UBlackboardComponent* blackboardComponent
	);

	FVector GetActorTraceLocation(AActor* thisActor) const;

	bool IsInLineOfSight(AActor* selfActor, AActor* otherActor);
	
	static bool IsAlive(AActor* actor);
};
