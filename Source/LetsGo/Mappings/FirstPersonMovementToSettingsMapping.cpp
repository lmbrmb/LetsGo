#include "FirstPersonMovementToSettingsMapping.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Movement/FirstPersonMovementComponent.h"

void UFirstPersonMovementToSettingsMapping::Map()
{
	AssertIsGreater(_mouseSensitivityMax, 0.0f);

	auto const owner = GetOwner();
	auto const firstPersonMovementComponent = owner->FindComponentByClass<UFirstPersonMovementComponent>();
	AssertIsNotNull(firstPersonMovementComponent);

	auto const authGameMode = GetWorld()->GetAuthGameMode();
	auto const matchGameMode = Cast<AMatchGameMode>(authGameMode);
	AssertIsNotNull(matchGameMode);

	auto const playerSettings = matchGameMode->GetPlayerSettings();
	auto const mouseSensitivity = playerSettings->MouseSensitivity;
	auto const mouseSensitivityAmount = mouseSensitivity / _mouseSensitivityMax;
	auto const cameraPitchSpeed = _cameraPitchSpeedMax * mouseSensitivityAmount;
	auto const actorYawSpeed = _actorYawSpeedMax * mouseSensitivityAmount;
	firstPersonMovementComponent->SetLookSpeed(cameraPitchSpeed, actorYawSpeed);

	StartDestroyTask();
}
