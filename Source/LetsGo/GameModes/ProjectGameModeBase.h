#pragma once

#include "GameFramework/GameModeBase.h"
#include "LetsGo/DiContainers/IDiContainerProvider.h"
#include "LetsGo/Utils/AssertUtils.h"

#include "ProjectGameModeBase.generated.h"

/// <summary>
/// Base project game mode 
/// </summary>
UCLASS()
class LETSGO_API AProjectGameModeBase : public AGameModeBase, public IDiContainerProvider
{
	GENERATED_BODY()

public:
	// Have to implement IDiContainerProvider, "cannot instantiate abstract class" error otherwise
	virtual TTypeContainer<ESPMode::Fast>* GetDiContainer() const override
	{
		AssertDefaultImplementationIsOverriden(nullptr);
	}
};
