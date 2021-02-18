#pragma once

#include "LoggingChannel.h"

/// <summary>
/// Debug logging channel implementation
/// </summary>
class LETSGO_API DebugLoggingChannel final : public LoggingChannel
{
public:
	explicit DebugLoggingChannel(int maxMessageCount, float timeToDisplay);

protected:
	///<summary>
	///[Template method] Implementation of how message will be logged (to screen, to file, etc.)
	///</summary>
	virtual void LogImplementation(const FString message, LogSeverity severity = LogSeverity::Debug) override;
	
private:
	int _maxMessageCount;

	float _timeToDisplay;
	
	int _lineIndex;

	bool _isMessageCountLimited;
	
	int _messageNumber = 0;

	static FColor GetSeverityColor(const LogSeverity severity);
};
