#pragma once

#include "LoggingChannelFactory.h"
#include "DebugLoggingChannel.h"

/// <summary>
/// Debug logging channel factory implementation
/// </summary>
class DebugLoggingChannelFactory : public LoggingChannelFactory
{
public:
	DebugLoggingChannelFactory();
	
	virtual ~DebugLoggingChannelFactory();

	virtual LoggingChannel* CreateLoggingChannel() override;
};
