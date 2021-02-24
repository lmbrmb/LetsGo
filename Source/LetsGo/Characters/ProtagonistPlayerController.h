#pragma once

#include "GameFramework/PlayerController.h"
#include "ProtagonistPawn.h"
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
	
private:
	AProtagonistPawn* _protagonistPawn = nullptr;

	void DebugDisplayPawnInfo() const;
};
