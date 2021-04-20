#pragma once

#include "MappingComponent.h"

#include "LetsGo/Movement/MovementComponentBase.h"
#include "LetsGo/Animations/CurveAnimationComponent.h"

#include "GunRunAnimationMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UGunRunAnimationMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:
	UGunRunAnimationMapping();

protected:
	virtual void Map() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _animationId;
	
	UMovementComponentBase* _movementComponent = nullptr;

	UCurveAnimationComponent* _curveAnimationComponent = nullptr;
	
	bool _isInitialized = false;

	FCurveAnimationState _state;
};
