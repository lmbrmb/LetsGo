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
		for(auto i = actor->Children.Num()-1; i >= 0; i--)
		{
			auto const child = actor->Children[i];
			DestroyActorRecursively(child);
		}

		actor->Destroy();
	}
};
