#pragma once

class ActorUtils
{
private:
	ActorUtils() = delete;

public:
	static void SetEnabled(AActor* actor, const bool isEnabled)
	{
		actor->SetActorHiddenInGame(!isEnabled);
		actor->PrimaryActorTick.bCanEverTick = isEnabled;
		actor->SetActorEnableCollision(isEnabled);
	}

	static void DestroyActorRecursively(AActor* actor)
	{
		for (auto child : actor->Children)
		{
			DestroyActorRecursively(child);
		}

		actor->Destroy();
	}
};
