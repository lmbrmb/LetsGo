#include "DebugLoggingChannelFactory.h"

const int MAX_MESSAGE_COUNT = -1;

const float TIME_TO_DISPLAY = 10.0f;

DebugLoggingChannelFactory::DebugLoggingChannelFactory()
{
}

DebugLoggingChannelFactory::~DebugLoggingChannelFactory()
{
}

LoggingChannel* DebugLoggingChannelFactory::CreateLoggingChannel()
{
	return new DebugLoggingChannel(MAX_MESSAGE_COUNT, TIME_TO_DISPLAY);
}
