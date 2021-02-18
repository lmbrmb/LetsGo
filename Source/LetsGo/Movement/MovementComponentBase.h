#pragma once
#include "LetsGo/Force.h"
#include "Components/ShapeComponent.h"
#include "MovementComponentBase.generated.h"

const bool DETECT_COLLISION_ON_MOVEMENT = true;
const FName GRAVITY_FORCE_NAME = FName("Gravity");
const FName JUMP_FORCE_ID = FName("Jump");

///<summary>
/// [Abstract] Movement component
///</summary>
UCLASS( Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	FHitResult _floorHitResult;

	FVector _previousLocation = FVector::ZeroVector;

	FVector _velocity = FVector::ZeroVector;

	void ProcessForces(float deltaTime);
	
	void CheckGround();

	void Translate(FVector translation);

	TArray<Force*> _forces;

	void UpdateVelocity();
};
