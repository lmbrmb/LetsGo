#pragma once

#include "LoggingChannel.h"

class LoggingChannelFactory
{
public:
	LoggingChannelFactory();

	virtual ~LoggingChannelFactory();

	virtual LoggingChannel* CreateLoggingChannel() = 0;
};
