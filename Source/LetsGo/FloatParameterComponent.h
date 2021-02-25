#pragma once

#include "Components/ActorComponent.h"
#include "FloatParameterComponent.generated.h"

UCLASS( Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETSGO_API UFloatParameterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFloatParameterComponent();

	DECLARE_EVENT(UHealth, EChanged)
	EChanged Changed;

	void SetCurrentValue(float currentValue);
	
	void SetMinValue(float minValue);
	
	void SetMaxValue(float maxValue);

	void MinimizeCurrentValue();
	
	void MaximizeCurrentValue();

	float GetMinValue() const;
	
	float GetMaxValue() const;

	float GetCurrentValue() const;

	float GetNormalizedValue() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float InitialValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MinValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxValue = 0;

	void Init();
	
	void ChangeValue(float amount);

	virtual void OnChanged(){};
};
