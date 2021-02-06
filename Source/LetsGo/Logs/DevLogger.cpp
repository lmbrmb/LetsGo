#include "DevLogger.h"
#include "DebugLoggingChannelFactory.h"

LoggingChannel* DevLogger::_loggingChannel = nullptr;

LoggingChannel* DevLogger::GetLoggingChannel()
{
	if (!_loggingChannel)
	{
		DebugLoggingChannelFactory factory;
		_loggingChannel = factory.CreateLoggingChannel();
	}
	return _loggingChannel;
}
