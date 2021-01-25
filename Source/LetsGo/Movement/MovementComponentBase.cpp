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
	auto const actor = GetAttachmentRootActor();
	Init(actor);
	
	auto const inputComponent = actor->InputComponent;
	if (inputComponent)
	{
		MapPlayerInput(inputComponent);
	}
}

void UMovementComponentBase::Init(AActor* actor)
{
}

void UMovementComponentBase::MapPlayerInput(UInputComponent* playerInputComponent)
{
}
