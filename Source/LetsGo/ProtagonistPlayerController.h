#pragma once

#include "GameFramework/PlayerController.h"
#include "ProtagonistPawn.h"
#include "Logs/LoggingChannel.h"
#include "ProtagonistPlayerController.generated.h"

///<summary>
/// Main character player controller
///</summary>
UCLASS()
class LETSGO_API AProtagonistPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	
protected:
	virtual void BeginPlay() override;

	LoggingChannel* GetLoggingChannel() const;
	
private:
	AProtagonistPawn* _protagonistPawn = nullptr;

	void DebugDisplayPawnInfo() const;
};
