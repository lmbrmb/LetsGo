#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ShapeComponent.h"
#include "LetsGo/Forces/ForceFactory.h"
#include "LetsGo/Forces/IForce.h"

#include "RigidBodyComponent.generated.h"

DECLARE_EVENT_OneParam(URigidBodyComponent, ELand, const float airTime);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API URigidBodyComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	URigidBodyComponent();

	void AddForce(
		const FName& id,
		const FVector& direction,
		const float magnitude
	);
	
	void AddForce(
		const FName& id,
		const FVector& direction,
		UCurveFloat* magnitudeCurve
	);

	void AddForce(
		const FName& id,
		const FVector& direction,
		UCurveFloat* magnitudeCurve,
		const float curveMagnitudeMultiplier,
		const float curveTimeMultiplier
	);

	void RemoveForce(const FName& id);

	bool GetIsInAir() const;

	const FVector& GetVelocity() const;

	ELand Land;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static const FName GRAVITY_FORCE_ID;

	UPROPERTY(EditAnywhere, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	float _gravityForceMagnitude = 981.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> _collisionChannel = ECC_Pawn;

	FHitResult _hitResult;

	TArray<IForce*> _forces;

	ForceFactory* _forceFactory;

	UShapeComponent* _rootCollider = nullptr;

	FCollisionShape _collisionShape;

	FCollisionQueryParams _collisionQueryParams;

	bool _isInAir;

	FVector _velocity = FVector::ZeroVector;

	FVector _previousLocation = FVector::ZeroVector;

	float _lastGroundTime = 0;

	void ProcessForces(const float deltaTime);

	void UpdateVelocity();
};
