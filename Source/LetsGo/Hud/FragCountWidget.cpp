#include "FragCountWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "LetsGo/GameModes/MatchGameMode.h"
#include "LetsGo/Utils/AssertUtils.h"

void UFragCountWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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
	auto const playerTeamId = _matchGameMode->GetPlayerTeamId(_localPlayerId);
	AssertIsTrue(playerTeamId.IsValid());
	auto const playerTeamIdValue = playerTeamId.GetId();
	
	// Team score == Player score in DeathMatch because each player has own team id

	auto playerTeamFragCount = 0;
	auto enemyTeamMaxFragCount = 0;
	
	for (auto const teamFragsRecord : _matchGameMode->GetTeamFrags())
	{
		auto const teamIdValue = teamFragsRecord.Key;
		auto const teamFrags = teamFragsRecord.Value;

		if(teamIdValue == playerTeamIdValue)
		{
			playerTeamFragCount = teamFrags;
		}
		else
		{
			if(enemyTeamMaxFragCount < teamFrags)
			{
				enemyTeamMaxFragCount = teamFrags;
			}
		}
	}
	
	BpOnFragCountChanged(playerTeamFragCount, enemyTeamMaxFragCount);
}
