#include "PickupItem.h"

FString APickupItem::GetId()
{
	return _id;
}

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;
	_root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(_root);
}
