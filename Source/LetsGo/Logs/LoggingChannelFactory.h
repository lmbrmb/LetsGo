#pragma once

#include "LoggingChannel.h"

/// <summary>
/// [Abstract] Logging channel factory contract
/// </summary>
class LoggingChannelFactory
{
public:
	LoggingChannelFactory() = default;

	virtual ~LoggingChannelFactory() = default;

	virtual LoggingChannel* CreateLoggingChannel() = 0;
};
