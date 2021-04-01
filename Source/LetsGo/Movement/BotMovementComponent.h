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
	bool IsTargetLocationValid() const;

	const FVector& GetTargetLocation() const;
	
	void SetTargetLocation(const FVector& targetLocation);

	void ClearTargetLocation();
	
protected:
	virtual FVector GetMovementDirection() override;

	virtual float GetMovementSpeed() override;

	virtual float GetAbsoluteMovementAmount() const override;
	
	virtual void Init(AActor* actor) override;
	
	virtual void ProcessInput() override;

	virtual void ResetInput() override;

	virtual void CustomTick(float deltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveForwardSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _actorMoveBackwardSpeed = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float _airMultiplier = 0.25f;

	bool _isTargetLocationValid = false;

	FVector _targetLocation = FVector::ZeroVector;
	
	FVector _inputMovementDirection = FVector::ZeroVector;
};
