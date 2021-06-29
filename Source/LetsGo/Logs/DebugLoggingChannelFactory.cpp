#include "DebugLoggingChannelFactory.h"

#include "DebugLoggingChannel.h"

const int MAX_MESSAGE_COUNT = -1;

const float TIME_TO_DISPLAY = 60.0f;

TUniquePtr<LoggingChannel> DebugLoggingChannelFactory::CreateLoggingChannel()
{
	return MakeUnique<DebugLoggingChannel>(MAX_MESSAGE_COUNT, TIME_TO_DISPLAY);
}
