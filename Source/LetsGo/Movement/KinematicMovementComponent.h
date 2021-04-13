#pragma once

#include "Components/ActorComponent.h"
#include "KinematicMovementComponent.generated.h"

DECLARE_EVENT_OneParam(UKinematicMovementComponent, ECollision_KinematicMovement, const FHitResult& hitResult)

/// <summary>
/// Kinematic movement component. No physics. Constant linear movement. Collision detection via ray cast.
/// </summary>
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UKinematicMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKinematicMovementComponent();

	ECollision_KinematicMovement Collision;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> _collisionChannel;

	UShapeComponent* _rootCollider = nullptr;

	FHitResult _bufferHitResult;

	FCollisionShape _collisionShape;

	FCollisionQueryParams _collisionQueryParams;

	FVector _previousLocation;
};
