#include "DebugLoggingChannel.h"
#include "Kismet/KismetStringLibrary.h"

DebugLoggingChannel::DebugLoggingChannel(int maxMessageCount)
{
	_maxMessageCount = maxMessageCount;
}

void DebugLoggingChannel::Log(FString str)
{
	_messageNumber++;
	
	if (!GEngine)
		return;

	auto const message = "[" + UKismetStringLibrary::Conv_IntToString(_messageNumber) + "] " + str;
	GEngine->AddOnScreenDebugMessage(_lineIndex, 10, FColor::Blue, message);
	_lineIndex++;
	if (_lineIndex >= _maxMessageCount)
	{
		_lineIndex = 0;
	}
}
