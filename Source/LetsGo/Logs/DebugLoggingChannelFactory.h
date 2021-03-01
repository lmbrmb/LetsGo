#pragma once

#include "LoggingChannelFactory.h"

/// <summary>
/// Debug logging channel factory implementation
/// </summary>
class DebugLoggingChannelFactory final : public LoggingChannelFactory
{
public:
	DebugLoggingChannelFactory() = default;
	
	virtual ~DebugLoggingChannelFactory() = default;

	virtual LoggingChannel* CreateLoggingChannel() override;
};
