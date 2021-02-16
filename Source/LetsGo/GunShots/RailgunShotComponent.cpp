#include "RailgunShotComponent.h"
#include "Components/LineBatchComponent.h"

void URailgunShotComponent::Init()
{
	_lineBatcher = GetWorld()->LineBatcher;
}

void URailgunShotComponent::OnShot(FTransform pivotTransform, FVector shotDirection)
{
	//TODO: draw ray, check hit
}
