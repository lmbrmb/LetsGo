#pragma once

#include "GameFramework/PlayerController.h"
#include "LetsGo/Avatars/Avatar.h"

#include "MatchPlayerController.generated.h"

///<summary>
/// Match player controller
///</summary>
UCLASS()
class LETSGO_API AMatchPlayerController final : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

	AAvatar* GetAvatar() const;
	
private:
	AAvatar* _avatar = nullptr;
};
