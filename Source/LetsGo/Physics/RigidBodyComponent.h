#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ShapeComponent.h"
#include "LetsGo/Forces/ForceFactory.h"
#include "LetsGo/Forces/IForce.h"

#include "RigidBodyComponent.generated.h"

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
	
protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static const FName GRAVITY_FORCE_ID;
	
	UPROPERTY(EditAnywhere, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	float _gravityForceMagnitude = 981.0f;

	TArray<IForce*> _forces;

	ForceFactory* _forceFactory;

	UShapeComponent* _rootCollider = nullptr;
	
	void ProcessForces(const float deltaTime);
};
