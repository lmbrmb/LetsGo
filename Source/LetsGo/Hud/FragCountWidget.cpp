#include "FragCountWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UFragCountWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	auto const world = GetWorld();
	auto const authGameMode = world->GetAuthGameMode();
	_matchGameMode = Cast<AMatchGameMode>(authGameMode);

	AssertIsNotNull(_matchGameMode);

	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AssertIsNotNull(localPlayerController);

	auto const localPlayerState = localPlayerController->GetPlayerState<APlayerState>();
	AssertIsNotNull(localPlayerState);
	
	auto const localPlayerIdValue = localPlayerState->GetPlayerId();
	
	_localPlayerId = PlayerId(localPlayerIdValue);
	
	_matchGameMode->PlayerFragged.AddUObject(this, &UFragCountWidget::OnPlayerFragged);
}

void UFragCountWidget::OnPlayerFragged(
	const PlayerId& instigatorPlayerId,
	const PlayerId& fraggedPlayerId
)
{
	if(instigatorPlayerId != _localPlayerId)
	{
		return;
	}
	auto const instigatorPlayerTeamId = _matchGameMode->GetPlayerTeamId(instigatorPlayerId);
	
	AssertIsTrue(instigatorPlayerTeamId.IsValid());

	// Team score == Player score in DeathMatch because each player has own team id
	auto const teamFragCount = _matchGameMode->GetTeamFragCount(instigatorPlayerTeamId);
	BpOnFragCountChanged(teamFragCount);
}
