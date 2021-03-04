#pragma once

#include "GameFramework/PlayerController.h"
#include "LetsGo/Avatars/Avatar.h"

#include "ProtagonistPlayerController.generated.h"

///<summary>
/// Main character player controller
///</summary>
UCLASS()
class LETSGO_API AProtagonistPlayerController final : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

	AAvatar* GetAvatar();
	
private:
	AAvatar* _avatar = nullptr;
};
