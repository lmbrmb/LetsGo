#pragma once

#include "IAimProvider.h"
#include "Components/ActorComponent.h"

#include "AimProviderComponent.generated.h"

DECLARE_EVENT(UAimProviderComponent, EChanged_UAimProviderComponent);

/// <summary>
/// Aim provider component. IAimProvider container
/// </summary>
UCLASS( Abstract, ClassGroup=(Custom) )
class LETSGO_API UAimProviderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAimProviderComponent();

	virtual ~UAimProviderComponent();

	EChanged_UAimProviderComponent Changed;

	IAimProvider* GetAimProvider() const;

protected:
	void SetAimProvider(IAimProvider* aimProvider);
	
private:
	IAimProvider* _aimProvider;
};
