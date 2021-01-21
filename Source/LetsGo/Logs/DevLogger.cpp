#include "DevLogger.h"
#include "DebugLoggingChannelFactory.h"

DevLogger::DevLogger()
{
}

DevLogger::~DevLogger()
{
}

LoggingChannel* DevLogger::_loggingChannel = nullptr;

LoggingChannel* DevLogger::GetLoggingChannel()
{
	if (_loggingChannel == nullptr)
	{
		/*auto factory = new DebugLoggingChannelFactory();
		_loggingChannel = factory->CreateLoggingChannel();
		delete factory;*/

		DebugLoggingChannelFactory factory;
		_loggingChannel = factory.CreateLoggingChannel();
	}
	return _loggingChannel;
}
