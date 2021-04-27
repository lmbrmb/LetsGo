#include "InputToSpectatorMovementMapping.h"

#include "GameFramework/SpectatorPawnMovement.h"
#include "LetsGo/Utils/AssertUtils.h"

void UInputToSpectatorMovementMapping::Map()
{
	auto const owner = GetOwner();

	_matchSpectatorPawn = Cast<AMatchSpectatorPawn>(owner);
	AssertIsNotNull(_matchSpectatorPawn);
	
	auto const spectatorPawnMovement = owner->FindComponentByClass<USpectatorPawnMovement>();
	AssertIsNotNull(spectatorPawnMovement);
	
	_matchSpectatorPawn->Possessed.AddUObject(this, &UInputToSpectatorMovementMapping::OnPossessed);
	OnPossessed();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInputToSpectatorMovementMapping::OnPossessed()
{
	auto const inputComponent = _matchSpectatorPawn->InputComponent;
	if (!inputComponent)
	{
		//DevLogger::GetLoggingChannel()->Log("UInputToSpectatorMovementMapping::OnPossessed - No input component", LogSeverity::Error);
		return;
	}
}
