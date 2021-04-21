#include "AvatarAnimInstance.h"

#include "LetsGo/Utils/AssertUtils.h"

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

void UAvatarAnimInstance::OnDied(UHealthComponent* healthComponent, const float delta)
{
	AssertIsTrue(healthComponent == _healthComponent);

	_healthComponent->Died.RemoveAll(this);
	BpOnAvatarDied();
}

float UAvatarAnimInstance::GetMovementSpeedValue() const
{
	return _movementComponentBase ? _movementComponentBase->GetAbsoluteMovementAmount() : 0.0f;
}

bool UAvatarAnimInstance::GetIsInAirValue() const
{
	return _movementComponentBase ? _movementComponentBase->GetIsInAir() : false;
}
