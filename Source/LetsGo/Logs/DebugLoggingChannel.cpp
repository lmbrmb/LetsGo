#include "DebugLoggingChannel.h"
#include "Kismet/KismetStringLibrary.h"

DebugLoggingChannel::DebugLoggingChannel(int maxMessageCount, float timeToDisplay)
{
	_maxMessageCount = maxMessageCount;
	_timeToDisplay = timeToDisplay;
	_isMessageCountLimited = maxMessageCount >= 1;
	_lineIndex = _isMessageCountLimited ? 0 : -1;
}

void DebugLoggingChannel::Log(const FString message, LogSeverity::LogSeverity severity)
{
	_messageNumber++;
	
	if (!GEngine)
		return;

	auto const finalMessage = "[" + UKismetStringLibrary::Conv_IntToString(_messageNumber) + "] " + message;
	auto const color = GetSeverityColor(severity);
	GEngine->AddOnScreenDebugMessage(_lineIndex, _timeToDisplay, color, finalMessage);

	if(_isMessageCountLimited)
	{
		_lineIndex++;

		if (_lineIndex >= _maxMessageCount)
		{
			_lineIndex = 0;
		}
	}
}

FColor DebugLoggingChannel::GetSeverityColor(const LogSeverity::LogSeverity severity)
{
	switch (severity)
	{
	case  LogSeverity::Debug:
		return FColor::Emerald;
	case LogSeverity::Warning:
		return FColor::Orange;
	case LogSeverity::Error:
		return FColor::Red;
	default:
		return FColor::Magenta;
	};
}
