#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/HealthSystem/HealthComponent.h"

#include "HealthWidget.generated.h"

/// <summary>
/// Health HUD widget
/// </summary>
UCLASS()
class LETSGO_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAvatarChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAvatarHealthChanged();

	virtual void NativeOnInitialized() override;

private:
	void OnAvatarChanged(const AAvatar* avatar);

	void OnAvatarHealthChanged(UHealthComponent* healthComponent, float delta);

	UHealthComponent* _healthComponent = nullptr;
};
