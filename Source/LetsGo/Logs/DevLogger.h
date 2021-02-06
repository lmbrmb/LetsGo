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
	
	static LoggingChannel* _loggingChannel;
};
