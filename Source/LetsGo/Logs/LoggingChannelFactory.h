#pragma once

#include "LoggingChannel.h"

class LoggingChannelFactory
{
public:
	LoggingChannelFactory();

	virtual ~LoggingChannelFactory();

	virtual LoggingChannel* GetOrCreateLoggingChannel() = 0;
};
