#pragma once

#include "GameFramework/HUD.h"

#include "Blueprint/UserWidget.h"

#include "PlayerHud.generated.h"

DECLARE_EVENT(APlayerHud, EInitialized_PlayerHud);

/// <summary>Player HUD</summary>
UCLASS()
class LETSGO_API APlayerHud : public AHUD
{
	GENERATED_BODY()

public:
	UUserWidget* GetHudWidget() const;

	bool IsInitialized() const;
	
	EInitialized_PlayerHud Initialized;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> _hudWidgetBlueprint;

	UUserWidget* _hudWidget = nullptr;
};
