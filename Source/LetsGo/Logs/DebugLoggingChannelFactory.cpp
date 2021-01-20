#include "DebugLoggingChannelFactory.h"

const int MAX_MESSAGE_COUNT = 3;

DebugLoggingChannelFactory::DebugLoggingChannelFactory()
{
}

DebugLoggingChannelFactory::~DebugLoggingChannelFactory()
{
}

LoggingChannel* DebugLoggingChannelFactory::CreateLoggingChannel()
{
	if(_debugLoggingChannel == nullptr)
	{
		_debugLoggingChannel = new DebugLoggingChannel(MAX_MESSAGE_COUNT);
	}
	
	return _debugLoggingChannel;
}
