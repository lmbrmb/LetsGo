#pragma once

#include "LoggingChannel.h"

/// <summary>
/// Debug logging channel implementation
/// </summary>
class LETSGO_API DebugLoggingChannel final : public LoggingChannel
{
public:
	explicit DebugLoggingChannel(int maxMessageCount);

	virtual void Log(FString str) override;

private:
	int _lineIndex = 0;

	int _maxMessageCount = 0;

	int _messageNumber = 0;
};