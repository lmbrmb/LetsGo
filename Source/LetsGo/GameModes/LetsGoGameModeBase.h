#pragma once

#include "GameFramework/GameModeBase.h"
#include "Misc/TypeContainer.h"
#include "LetsGoGameModeBase.generated.h"

///<summary>
///Game mode base. Has Dependency Injection container.
///</summary>
UCLASS()
class LETSGO_API ALetsGoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALetsGoGameModeBase();
	
	virtual ~ALetsGoGameModeBase();

	TTypeContainer<ESPMode::Fast>* GetDiContainer() const;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
private:
	TTypeContainer<ESPMode::Fast>* _diContainer = nullptr;

	void ComposeDiContainer() const;
};
