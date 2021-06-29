#pragma once

#include "LoggingChannel.h"

/// <summary>
/// Development logger
/// </summary>
class DevLogger
{
public:
	static LoggingChannel* GetLoggingChannel();
	
private:
	DevLogger() = delete;
	
	static TUniquePtr<LoggingChannel> _loggingChannel;
};
