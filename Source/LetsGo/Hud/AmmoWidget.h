#pragma once

#include "Blueprint/UserWidget.h"
#include "LetsGo/Avatars/Avatar.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

#include "AmmoWidget.generated.h"

/// <summary>
/// Ammo HUD widget
/// </summary>
UCLASS()
class LETSGO_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BpOnGunChanged(const FName& ammoId);

	UFUNCTION(BlueprintImplementableEvent)
	void BpOnAmmoCountChanged(const int ammoCount);

	virtual void NativeConstruct() override;

private:
	void OnAvatarChanged(const AAvatar* avatar);

	void OnWeaponChanged();

	void OnAmmoCountChanged(const int ammoCount);

	IGun* _gun;

	UWeaponManagerComponent* _weaponManagerComponent = nullptr;
};
