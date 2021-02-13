#pragma once

#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

/// <summary>Player HUD</summary>
UCLASS()
class LETSGO_API APlayerHud : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> _hudBlueprint;

	UUserWidget* _hud;
};
