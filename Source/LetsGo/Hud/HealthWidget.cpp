#include "HealthWidget.h"

#include "Kismet/GameplayStatics.h"
#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/PlayerControllers/MatchPlayerController.h"
#include "LetsGo/Utils/AssertUtils.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	auto const localPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AssertIsNotNull(localPlayerController);

	auto const matchPlayerController = Cast<AMatchPlayerController>(localPlayerController);

	AssertIsNotNull(matchPlayerController);

	matchPlayerController->AvatarChanged.AddUObject(this, &UHealthWidget::OnAvatarChanged);
	auto const avatar = matchPlayerController->GetAvatar();
	OnAvatarChanged(avatar);
}

void UHealthWidget::OnAvatarChanged(const AAvatar* avatar)
{
	if(_healthComponent)
	{
		_healthComponent->HealthChanged.RemoveAll(this);
	}

	if(avatar)
	{
		_healthComponent = avatar->FindComponentByClass<UHealthComponent>();

		if (_healthComponent)
		{
			_healthComponent->HealthChanged.AddUObject(this, &UHealthWidget::OnAvatarHealthChanged);
		}
	}

	BpOnAvatarChanged();
}

void UHealthWidget::OnAvatarHealthChanged(UHealthComponent* healthComponent, float delta)
{
	BpOnAvatarHealthChanged();
}

UHealthComponent* UHealthWidget::GetHealthComponent()
{
	return _healthComponent;
}
