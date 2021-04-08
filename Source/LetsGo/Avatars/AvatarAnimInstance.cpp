#include "AvatarAnimInstance.h"

#include "LetsGo/Utils/AssertUtils.h"

UHealthComponent* UAvatarAnimInstance::GetHealthComponent()
{
	return _healthComponent;
}

UMovementComponentBase* UAvatarAnimInstance::GetMovementComponent()
{
	return _movementComponentBase;
}

void UAvatarAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	auto const owner = GetOwningActor();
	AssertIsNotNull(owner);

	_healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(_healthComponent);
	
	_healthComponent->Died.AddUObject(this, &UAvatarAnimInstance::OnDied);

	_movementComponentBase = owner->FindComponentByClass<UMovementComponentBase>();
	AssertIsNotNull(_movementComponentBase);
}

void UAvatarAnimInstance::OnDied(const UHealthComponent*, const float delta)
{
	BpOnAvatarDied();
}
