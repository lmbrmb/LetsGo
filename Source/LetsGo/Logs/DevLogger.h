#pragma once

#include "LoggingChannel.h"

static class DevLogger
{
public:
	static LoggingChannel* GetLoggingChannel();
	
private:
	DevLogger();

	~DevLogger();
	
	static LoggingChannel* _loggingChannel;
};
