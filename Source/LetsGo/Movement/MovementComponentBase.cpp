#include "MovementComponentBase.h"

UMovementComponentBase::UMovementComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UMovementComponentBase::~UMovementComponentBase()
{
}

void UMovementComponentBase::BeginPlay()
{
	Super::BeginPlay();
	auto const actor = GetOwner();
	Init(actor);

	auto const inputComponent = actor->InputComponent;
	if (inputComponent)
	{
		MapPlayerInput(inputComponent);
	}
}
