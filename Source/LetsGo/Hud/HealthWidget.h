#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

#include "HealthWidget.generated.h"

/// <summary>
/// Health hud widget
/// </summary>
UCLASS()
class LETSGO_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAvatarChanged();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAvatarHealthChanged();
	
protected:
	virtual void NativeConstruct() override;
	
private:
	void OnAvatarChanged(const AAvatar* avatar);
	
	void OnAvatarHealthChanged(const UHealthComponent*, float delta);

	UHealthComponent* _healthComponent = nullptr;
};
