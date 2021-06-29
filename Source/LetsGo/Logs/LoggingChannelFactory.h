#pragma once

class LoggingChannel;

/// <summary>
/// [Abstract] Logging channel factory contract
/// </summary>
class LoggingChannelFactory
{
public:
	LoggingChannelFactory() = default;

	virtual ~LoggingChannelFactory() = default;

	virtual TUniquePtr<LoggingChannel> CreateLoggingChannel() = 0;
};
