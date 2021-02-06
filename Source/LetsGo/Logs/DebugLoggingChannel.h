#pragma once

#include "LoggingChannel.h"

/// <summary>
/// Debug logging channel implementation
/// </summary>
class LETSGO_API DebugLoggingChannel final : public LoggingChannel
{
public:
	explicit DebugLoggingChannel(int maxMessageCount, float timeToDisplay);

	virtual void Log(const FString message, LogSeverity severity = LogSeverity::Debug) override;

private:
	int _maxMessageCount;

	float _timeToDisplay;
	
	int _lineIndex;

	bool _isMessageCountLimited;
	
	int _messageNumber = 0;

	static FColor GetSeverityColor(const LogSeverity severity);
};
