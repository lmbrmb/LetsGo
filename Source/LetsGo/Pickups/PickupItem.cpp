#include "PickupItem.h"

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;
	_root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(_root);
}

FName APickupItem::GetId() const
{
	return _id;
}

void APickupItem::Take()
{
	Taken.Broadcast(this);
	_isTaken = true;
	BpOnTaken();
}

bool APickupItem::IsTaken() const
{
	return _isTaken;
}

void APickupItem::ResetTaken()
{
	_isTaken = false;
}
