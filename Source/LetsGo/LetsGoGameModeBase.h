#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Logs/LoggingChannel.h"
#include "Logs/LoggingChannelFactory.h"
#include "LetsGoGameModeBase.generated.h"

///<summary>
///Game mode base
///</summary>
UCLASS()
class LETSGO_API ALetsGoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALetsGoGameModeBase();
	
	~ALetsGoGameModeBase();

	LoggingChannel* GetLoggingChannel() const;

private:
	LoggingChannelFactory* _loggingChannelFactory;

	LoggingChannel* _loggingChannel;
};
