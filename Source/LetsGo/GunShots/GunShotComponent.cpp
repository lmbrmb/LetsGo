#include "GunShotComponent.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Utils/AssertUtils.h"

UGunShotComponent::UGunShotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	_audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	_audioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UGunShotComponent::SetWeaponId(const WeaponId& weaponId)
{
	InstigatorWeaponId = weaponId;
}

void UGunShotComponent::SetPlayerId(const PlayerId& instigatorId)
{
	InstigatorPlayerId = instigatorId;
}

void UGunShotComponent::SetAimProvider(USceneComponent* aimProvider)
{
	AimProvider = aimProvider;
}

bool UGunShotComponent::GetIsLocalPlayer() const
{
	return _isLocalPlayer;
}

void UGunShotComponent::BeginPlay()
{
	Super::BeginPlay();

	GunActor = GetOwner();
	AssertIsNotNull(GunActor);

	GunOwner = GunActor->GetOwner();
	AssertIsNotNull(GunOwner);

	auto const localPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (localPlayerPawn)
	{
		_isLocalPlayer = GunOwner == localPlayerPawn;
	}
	
	BpInit();
}
