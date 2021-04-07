#pragma once

#include "MovementComponentBase.h"
#include "BotMovementComponent.generated.h"

/// <summary>
/// Bot movement component
/// </summary>
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UBotMovementComponent final : public UMovementComponentBase
{
	GENERATED_BODY()

public:
	inline const FVector& GetTargetLocation() const;

	inline bool IsTargetLocationValid() const;

	inline void SetTargetLocation(const FVector& targetLocation);

	inline void ClearTargetLocation();

	inline const FVector& GetTargetRotation() const;

	inline bool IsTargetRotationValid() const;
	
	inline void SetTargetRotation(const FVector& targetRotationLocation);

	inline void ClearTargetRotation();
	
protected:
	virtual FVector GetMovementDirection() override;

	virtual float GetMovementSpeed() override;

	virtual float GetAbsoluteMovementAmount() const override;
	
	virtual void Init(AActor* actor) override;
	
	virtual void ProcessInput() override;

	virtual void ResetInput() override;

	virtual void CustomTick(const float deltaTime) override;
	
private:
	const float MIN_DOT_FORWARD = -0.01f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _airMultiplier = 0.25f;

	bool _isTargetLocationValid = false;
	
	FVector _targetLocation = FVector::ZeroVector;

	bool _isTargetRotationLocationValid = false;

	FVector _targetRotationLocation = FVector::ZeroVector;
	
	FVector _inputMovementDirection = FVector::ZeroVector;

	void ProcessTargetRotation(const float deltaTime) const;
};
