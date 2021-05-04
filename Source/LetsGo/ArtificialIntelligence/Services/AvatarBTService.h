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
	FName _pickupActorKeyName = "PickupActor";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = BlackboardKeys)
	FName _isEnemyInLineOfSightKeyName = "IsEnemyInLineOfSight";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Trace)
	float _detectionRadius = 2000;

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
	
	FCollisionShape _collisionShape;

	bool _isCollisionShapeInitialized = false;
	
	FHitResult _hitResult;

	TArray<FHitResult> _hitResults;

	TArray<AAvatar*> _enemies;

	TArray<APickupItem*> _pickupItems;

	void Update(UBlackboardComponent* blackboardComponent);

	static bool IsNeedHealth(UHealthComponent* healthComponent);

	bool IsNeedWeapon(UWeaponManagerComponent* weaponManagerComponent);

	bool IsLowTierWeapon(IWeapon* weapon) const;

	bool IsEnoughAmmo(IGun* gun);

	void Detect(
		AAvatar* selfAvatar,
		const FVector& selfLocation,
		const bool needHealth,
		const bool needWeapon
	);
	
	bool TryProcessEnemy(
		AActor* otherActor,
		const TeamId& selfTeamId
	);

	bool TryProcessPickup(
		const bool needHealth,
		const bool needWeapon,
		AActor* otherActor
	);

	bool CanTakePickup(
		const bool needHealth,
		const bool needWeapon,
		const APickupItem* pickupItem
	) const;
	
	void UpdateEnemy(
		UBlackboardComponent* blackboardComponent,
		AAvatar* selfAvatar,
		const FVector& selfLocation
	);

	void UpdatePickup(
		UBlackboardComponent* blackboardComponent,
		const bool needHealth,
		const bool needWeapon,
		const FVector& selfLocation
	) const;
	
	AAvatar* GetCurrentEnemy(
		UBlackboardComponent* blackboardComponent
	);

	FVector GetActorTraceLocation(AActor* thisActor) const;

	bool IsInLineOfSight(AActor* leftActor, AActor* rightActor);
	
	static bool IsAlive(AActor* actor);
};
