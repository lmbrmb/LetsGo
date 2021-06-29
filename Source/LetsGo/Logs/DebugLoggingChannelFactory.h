#pragma once

#include "LoggingChannelFactory.h"

/// <summary>
/// Debug logging channel factory implementation
/// </summary>
class DebugLoggingChannelFactory final : public LoggingChannelFactory
{
public:
	DebugLoggingChannelFactory() = default;
	
	virtual ~DebugLoggingChannelFactory() override = default;

	virtual TUniquePtr<LoggingChannel> CreateLoggingChannel() override;
};
