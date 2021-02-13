#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LetsGoGameModeBase.generated.h"

///<summary>
///Game mode base
///</summary>
UCLASS()
class LETSGO_API ALetsGoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALetsGoGameModeBase();
	
	~ALetsGoGameModeBase();

	//TODO: Dependency Injection container
};
