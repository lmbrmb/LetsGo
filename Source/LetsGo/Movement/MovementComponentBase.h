#pragma once

#include "MovementInfoProvider.h"
#include "Components/ShapeComponent.h"
#include "LetsGo/Forces/IForce.h"
#include "LetsGo/Forces/ForceFactory.h"

#include "MovementComponentBase.generated.h"

//<summary>
/// [Abstract] Movement component
///</summary>
UCLASS( ClassGroup=(Custom))
class LETSGO_API UMovementComponentBase : public UActorComponent, public IMovementInfoProvider
{
	GENERATED_BODY()

public:
	/// <summary>
	/// IMovementInfoProvider.GetIsInAir default implementation
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual float GetAbsoluteMovementAmount() override;
	
	/// <summary>
	/// IMovementInfoProvider.GetIsInAir implementation
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual bool GetIsInAir() const override;

	void Jump();
	
protected:
	static const FName GRAVITY_FORCE_ID;

	static const FName JUMP_FORCE_ID;

	static const float SLOPE_ZERO;

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
	
private:
	bool _isInAir = false;
	
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceCurveMagnitudeMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	float _jumpForceCurveTimeMultiplier = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _jumpForceUpCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* _jumpForceVelocityCurve;
	
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (AllowPrivateAccess = "true"))
	int _jumpCount = 1;

	int _jumpIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _gravityForceMagnitude = 981.0f;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	float _maxStepHeight = 20;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	float _maxSlopeDegrees = 45;
	
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
		const float translationAmount,
		int callNumber
	);

	TArray<IForce*> _forces;

	ForceFactory* _forceFactory;
	
	void UpdateVelocity();
};
