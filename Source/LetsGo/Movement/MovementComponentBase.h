#pragma once
#include "LetsGo/Force.h"
#include "Components/ShapeComponent.h"
#include "MovementComponentBase.generated.h"

const FName GRAVITY_FORCE_NAME = FName("Gravity");
const FName JUMP_FORCE_ID = FName("Jump");

///<summary>
/// [Abstract] Movement component
///</summary>
UCLASS( Abstract, ClassGroup=(Custom) )
class LETSGO_API UMovementComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Returns movement amount. Range [0..1]
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual float GetAbsoluteMovementAmount();

	void Jump();
	
protected:	
	UMovementComponentBase();

	virtual ~UMovementComponentBase();

	virtual void BeginPlay() override final;

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
	virtual FVector GetInputMovementDirection();

	/// <summary>
	/// [Template method] Returns movement speed
	/// </summary>
	virtual float GetMovementSpeed();
	
	/// <summary>
	/// [Template method] Performs custom actions in Tick function
	/// </summary>
	virtual void CustomTick(float deltaTime);
	
	UWorld* World = nullptr;

	USceneComponent* Root = nullptr;

	UShapeComponent* RootCollider = nullptr;

	FCollisionShape CollisionShape;

	FCollisionQueryParams CollisionQueryParams;

	bool IsInAir = false;
	
private:
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceMagnitude = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceDuration = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	int _jumpCount = 1;

	int _jumpIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _gravityForceMagnitude = 981.0f;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	float _maxStepHeight = 20;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	float _maxSlopeDegrees = 45;

	const float SLOPE_ZERO = 90.0f;
	
	float _maxSlopeDegreesUp = SLOPE_ZERO + _maxSlopeDegrees;
	
	float _maxSlopeDegreesDown = SLOPE_ZERO - _maxSlopeDegrees;

	FVector _previousLocation = FVector::ZeroVector;

	FVector _velocity = FVector::ZeroVector;

	/// <summary>
	/// Cached ground hit result. Updated every frame
	/// </summary>
	FHitResult _groundHitResult;

	/// <summary>
	/// Cached hit result for Translate() method
	/// </summary>
	FHitResult _bufferHitResult;
	
	FHitResult hitResult;
	
	void ProcessForces(const float& deltaTime);

	void ProcessMovement(const float& deltaTime);
	
	void CheckGround();

	const int FIRST_MOVE_CALL_NUMBER = 1;
	
	const int MAX_MOVE_CALL_DEPTH = 2;

	/// <summary>
	/// [Recursive] Moves Root in provided direction. Handles collision detection
	/// </summary>
	/// <param name="rootLocation">Root location</param>
	/// <param name="rootRotation">Root rotation</param>
	/// <param name="direction">Normalized target movement direction</param>
	/// <param name="planeHitResult">Hit result. Ground / obstacle of previous call </param>
	/// <param name="translationAmount">Length of translation vector (delta time should be already applied)</param>
	/// <param name="callNumber">Current call number. Will be increased each time this function is called recursively</param>
	void Move(
		const FVector& rootLocation,
		const FQuat& rootRotation,
		const FVector& direction,
		const FHitResult& planeHitResult,
		const float& translationAmount,
		int callNumber
	);

	TArray<Force*> _forces;

	void UpdateVelocity();
};
