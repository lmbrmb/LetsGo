#pragma once

#include "GameFramework/GameModeBase.h"
#include "LetsGo/Data/IUObjectRegistry.h"
#include "LetsGo/DiContainers/IDiContainerProvider.h"
#include "LetsGo/Utils/AssertUtils.h"

#include "ProjectGameModeBase.generated.h"

/// <summary>
/// Base project game mode 
/// </summary>
UCLASS()
class LETSGO_API AProjectGameModeBase : public AGameModeBase, public IDiContainerProvider, public IUObjectRegistry
{
	GENERATED_BODY()

public:
	// Have to implement IDiContainerProvider, "cannot instantiate abstract class" error otherwise
	virtual TTypeContainer<ESPMode::Fast>* GetDiContainer() const override
	{
		AssertDefaultImplementationIsOverriden(nullptr);
	}

	// IUObjectRegistry implementation
	virtual void RegisterUObject(UObject* uObject) override;

	// IUObjectRegistry implementation
	virtual void UnRegisterUObject(UObject* uObject) override;

private:
	// "UPROPERTY()" is a must have attribute. Garbage collector will not collect objects of this array while this game mode exists
	UPROPERTY()
	TArray<UObject*> _uObjectRegistry;
};
