#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "LetsGo/Movement/BotMovementComponent.h"

#include "MoveToEnemyBTTaskNode.generated.h"

/// <summary>
/// Move to enemy task node
/// </summary>
UCLASS()
class LETSGO_API UMoveToEnemyBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _selfActorKeyName = "SelfActor";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _enemyAvatarKeyName = "EnemyAvatar";

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	TEnumAsByte<ECollisionChannel> _collisionChannel = ECC_Pawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FVector _avatarRayCastLocationOffset = FVector::UpVector * 100;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _locationTolerance = 10;

	float _locationToleranceSquared = _locationTolerance * _locationTolerance;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	float _enemyDistance = 1000;
	
	float _enemyDistanceSquared = _enemyDistance * _enemyDistance;

	FHitResult _hitResult;
	
	EBTNodeResult::Type TaskFailed(UBotMovementComponent* botMovementComponent);
};
