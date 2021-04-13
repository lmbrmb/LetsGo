#include "ProjectileShotComponent.h"

void UProjectileShotComponent::OnShotRequested(const USceneComponent* firePivot)
{
	ShotPerformed.Broadcast(firePivot);
}
