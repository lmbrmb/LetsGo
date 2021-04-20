#pragma once

#include "MappingComponent.h"

#include "LetsGo/Animations/CurveAnimationComponent.h"

#include "LetsGo/Data/PlayerId.h"
#include "LetsGo/Data/WeaponType.h"
#include "GunRecoilAnimationMapping.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class LETSGO_API UGunRecoilAnimationMapping final : public UMappingComponent
{
	GENERATED_BODY()

public:
	UGunRecoilAnimationMapping();

protected:
	virtual void Map() override;
	
private:
	UPROPERTY(EditAnywhere, Category = Custom, meta = (AllowPrivateAccess = "true"))
	FName _animationId;
	
	UCurveAnimationComponent* _curveAnimationComponent = nullptr;
	
	bool _isInitialized = false;

	FCurveAnimationState _state;

	void OnShotPerformed(
		const PlayerId& playerId,
		const WeaponType& weaponType,
		const bool isAnyBulletDamaged
	) const;

	void OnWeaponEquipped() const;
};
