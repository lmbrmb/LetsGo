#pragma once

#include "Components/ShapeComponent.h"
#include "LetsGo/Forces/IForce.h"
#include "LetsGo/Forces/ForceFactory.h"

#include "MovementComponentBase.generated.h"

//<summary>
/// [Abstract] Movement component
///</summary>
UCLASS( ClassGroup=(Custom) )
class LETSGO_API UMovementComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual float GetAbsoluteMovementAmount() const;
	
	UFUNCTION(BlueprintCallable)
	virtual bool GetIsInAir() const;

	void Jump();

	FVector GetRootColliderLocation() const;

protected:
	static const FName GRAVITY_FORCE_ID;

	static const FName JUMP_FORCE_ID;

	UMovementComponentBase();

	virtual ~UMovementComponentBase();

	virtual void BeginPlay() override final;

	void ProcessActorRotation(const float deltaTime, const FVector& direction) const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override final;

	/// <summary>
	/// [Template method] Initialization of custom dependencies
	/// </summary>
	virtual void Init(AActor* actor);
	
	/// <summary>
	/// [Template method] Processes input
	/// </summary>
	virtual void ProcessInput();

	/// <summary>
	/// [Template method] Resets input
	/// </summary>
	virtual void ResetInput();

	/// <summary>
	/// [Template method] Returns input movement direction
	/// </summary>
	virtual FVector GetMovementDirection();

	/// <summary>
	/// [Template method] Returns movement speed
	/// </summary>
	virtual float GetMovementSpeed();
	
	/// <summary>
	/// [Template method] Performs custom actions in Tick function
	/// </summary>
	virtual void CustomTick(const float deltaTime);
	
	UWorld* World = nullptr;

	UShapeComponent* RootCollider = nullptr;

	FCollisionShape CollisionShape;
	
	FCollisionQueryParams CollisionQueryParams;

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnJump();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnStep();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnLand();
	
private:
	const float SKIP_ROTATION_DOT = 0.99f;

	/// <summary>
	/// Rotation speed in degrees per second
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _rotationSpeedDegrees = 540.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> _collisionChannel;
	
	// Jump
	
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceCurveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceCurveTimeMultiplier = 1;
	
	UPROPERTY(EditAnywhere, Category="Jump", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _jumpForceUpCurve;
	
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _jumpForceVelocityCurve;
	
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	int _jumpCount = 1;

	int _jumpIndex = 0;

	// Speed
	
	UPROPERTY(EditAnywhere, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	float _gravityForceMagnitude = 981.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float _maxStepHeight = 50;

	FVector _previousLocation = FVector::ZeroVector;

	FVector _velocity = FVector::ZeroVector;

	/// <summary>
	/// Cached ground hit result. Updated every frame
	/// </summary>
	FHitResult _groundHitResult;

	/// <summary>
	/// Cached hit result
	/// </summary>
	FHitResult _bufferHitResult;
	
	FHitResult hitResult;
	
	void ProcessForces(const float& deltaTime);

	void ProcessMovement(const float& deltaTime);
	
	void CheckGround();

	/// <summary>
	/// [Recursive] Moves Root in provided direction. Handles collision detection
	/// </summary>
	/// <param name="rootColliderLocation">Root location</param>
	/// <param name="rootColliderRotation">Root rotation</param>
	/// <param name="inputDirection">Normalized target movement direction</param>
	/// <param name="groundHitResult">Hit result. Ground / obstacle of previous call </param>
	/// <param name="translationAmount">Length of translation vector (delta time should be already applied)</param>
	void Move(
		const FVector& rootColliderLocation,
		const FQuat& rootColliderRotation,
		const FVector& inputDirection,
		const FHitResult& groundHitResult,
		const float translationAmount
	);

	TArray<IForce*> _forces;

	ForceFactory* _forceFactory;
	
	void UpdateVelocity();

	bool _isInAir = false;

	void SetIsInAir(const bool isInAir);
	
	void StepOnTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _stepInterval;
};
