#pragma once

#include "MovementSpeedState.h"
#include "Components/ShapeComponent.h"
#include "LetsGo/Physics/RigidbodyComponent.h"

#include "MovementComponentBase.generated.h"

DECLARE_EVENT_OneParam(UMovementComponentBase, EStep, MovementSpeedState)

DECLARE_EVENT(UMovementComponentBase, EJump)

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
	bool GetIsInAir() const;

	/// <summary>
	/// [Template method] Returns input movement direction
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual const FVector& GetMovementDirection() const;

	void ActivateMovementSpeedState(MovementSpeedState movementSpeedState);

	void DeactivateMovementSpeedState(MovementSpeedState movementSpeedState);

	void PerformJump();

	FVector GetRootColliderLocation() const;

	EStep Step;

	EJump Jump;

protected:
	static const FName JUMP_FORCE_ID;
	UMovementComponentBase();
	virtual void BeginPlay() override final;

	virtual void BeginDestroy() override;

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
	/// [Template method] Returns movement speed
	/// </summary>
	virtual float GetBaseMovementSpeed();
	
	/// <summary>
	/// [Template method] Performs custom actions in Tick function
	/// </summary>
	virtual void CustomTick(const float deltaTime);

	UShapeComponent* RootCollider = nullptr;

	FCollisionShape CollisionShape;
	
	FCollisionQueryParams CollisionQueryParams;
	
private:
	const float SKIP_ROTATION_DOT = 0.99f;

	static MovementSpeedState _defaultMovementSpeedState;

	/// <summary>
	/// Rotation speed in degrees per second
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _rotationSpeedDegrees = 540.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> _collisionChannel = ECC_Pawn;
	
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

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float _maxStepHeight = 50;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _walkMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _sprintMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _airMultiplier = 0.25f;

	FVector _previousLocation = FVector::ZeroVector;

	FVector _velocity = FVector::ZeroVector;

	MovementSpeedState _movementSpeedState;

	float GetEnvironmentSpeedMultiplier() const;

	float GetSpeedStateMultiplier() const;

	/// <summary>
	/// Cached ground hit result. Updated every frame
	/// </summary>
	FHitResult _groundHitResult;

	/// <summary>
	/// Cached hit result
	/// </summary>
	FHitResult _bufferHitResult;
	
	FHitResult hitResult;

	void ProcessMovement(const float deltaTime);
	
	void CheckGround();

	void OnLand();

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
	
	void UpdateVelocity();
	
	void StepOnTimer() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _stepInterval;

	FTimerHandle _stepTimerHandle;

	URigidBodyComponent* _rigidBodyComponent;
};
